/**
 * @file Socket.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright * Copyright (C) 2024 Felix Schuelke
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
 */


#ifndef SOCKET_H
#define SOCKET_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <CharArray.h>

class Socket{
    public:
        /**
         * @brief Default-Constructor for a new Socket object
         * 
         */
        Socket(uint8_t _componentId, uint8_t _instanceId):
        componentId(_componentId),
        instanceId(_instanceId),
        txComponentId('\0'),
        txInstanceId('\0'),
        rxObject(nullptr),
        txObject(nullptr),
        _receivingFinished(false),
        _receivingActive(false)
        {};

        /**
         * @brief Add a new Object to the Socket that should be transmitted next. 
         * After imparting the Object, the socket determines the Component- and the Instance-ID of the TX-Object.
         * 
         * @param _txObject Pointer to Object to be transmitted by the socket
         * @param _txComponentId Component-Id of the Target-Component the Object should be sent to 
         * @param _txInstanceId Instance-Id of the Target-Component the Object should be sent to 
         */
        void setTxObject(CharArray* _txObject, uint8_t _txComponentId, uint8_t _txInstanceId){
            txObject = _txObject;
            txComponentId = _txComponentId;
            txInstanceId = _txInstanceId;
        };

        /**
         * @brief Get Pointer to the Rx Object object (for processing inside the component)
         * 
         * @return CharArray* Pointer to  Rx-Object
         */
        CharArray* getRxObject(){
            return rxObject;
        };

        /**
         * @brief Called by Component after it finished processing the received Object
         * 
         */
        void processedRxObject(){
            _receivingFinished = true;
        };

    private:
        uint8_t componentId;
        uint8_t instanceId;
        CharArray* rxObject;
        CharArray* txObject;
        uint8_t txComponentId;
        uint8_t txInstanceId;
        bool _receivingFinished;
        bool _receivingActive;
        friend void transmission(Socket& sendingSocket, Socket& receivingSocket);
};


void transmission(Socket& sendingSocket, Socket& receivingSocket){
    // Continue for matching Identifier 
    if (receivingSocket.componentId == sendingSocket.txComponentId  && receivingSocket.instanceId == sendingSocket.instanceId){
        // Establish connection
        if (!receivingSocket._receivingActive && !receivingSocket._receivingFinished){
            // Transmission active till receiving Component processed Object 
            receivingSocket._receivingActive = true;

            // set the receive-Object-pointer to the Object the tx-Object-pointer is pointing at 
            receivingSocket.rxObject = sendingSocket.txObject;

        // End connection
        }else if (receivingSocket._receivingActive && receivingSocket._receivingFinished)
        {
            // Reset rx- and tx-Pointer 
            receivingSocket.rxObject = nullptr;
            sendingSocket.txObject = nullptr;

            // Reset Target IDs of sending Socket
            sendingSocket.txComponentId = '\0';
            sendingSocket.txInstanceId = '\0';

            // Reset Transmission-flags 
            receivingSocket._receivingActive = false;
            receivingSocket._receivingFinished = false;
        }
    } 
    return;
}

#endif // SOCKET_H