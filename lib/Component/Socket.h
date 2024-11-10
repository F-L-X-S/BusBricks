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

template<typename transmissionObjectType>  
class Socket{
    public:
        /**
         * @brief Default-Constructor for a new Socket object
         * 
         */
        Socket():
        rxObject(nullptr),
        txObject(transmissionObjectType()),
        _transmissionFinished(true)
        {};

        /**
         * @brief Transmit a Object to the target-socket 
         * 
         * @param targetSocket Pointer to Socket to transmit to
         */
        void transmitTo(Socket* targetSocket){
            if (targetSocket->readyToReceive()) targetSocket->impartRxObject(&txObject);
        };

        /**
         * @brief Transmit a Object to the Socket by setting the rxObject-Pointer to the specified Object
         * 
         * @param obj Object to to add to Socket
         */
        void impartRxObject(transmissionObjectType* obj){
            rxObject = obj;
        };

        /**
         * @brief Add a new Object to the Socket that should be transmitted next 
         * 
         * @param _txObject Object to be transmitted by the socket
         * @param _txComponentId Component-Id of the Target-Component the Object should be sent to 
         * @param _txInstanceId Instance-Id of the Target-Component the Object should be sent to 
         */
        void impartTxObject(transmissionObjectType _txObject, uint8_t _txComponentId, uint8_t _txInstanceId){
            txObject = _txObject;
            txComponentId = _txComponentId;
            txInstanceId = _txInstanceId;
        };

        /**
         * @brief Check, if Socket is ready to receive a new Object
         * 
         * @return true Socket is ready to receive a new Object
         * @return false Component has not finished processing the received Object
         */
        bool readyToReceive(){
            return rxObject == nullptr;
        }

        /**
         * @brief Check, if Socket is ready to transmit a new Object
         * 
         * @return true Socket is ready to transmit a new Object
         * @return false Component has not finished transmitting
         */
        bool readyToTransmit(){
            return _transmissionFinished;
        }

    protected:
        /**
         * @brief Get Pointer to the Rx Object object (for processing inside the component)
         * 
         * @return transmissionObjectType* Pointer to  Rx-Object
         */
        transmissionObjectType* getRxObject(){
            return &rxObject;
        };

        /**
         * @brief Called by Component after it finished processing the received Object
         * 
         */
        void processedRxObject(){
            rxObject = nullptr;
        };

    private:
        transmissionObjectType* rxObject;
        transmissionObjectType txObject;
        uint8_t txComponentId;
        uint8_t txInstanceId;
        bool _transmissionFinished;
};


#endif // SOCKET_H