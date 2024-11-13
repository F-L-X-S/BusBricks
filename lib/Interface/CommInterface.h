/**
 * @file CommInterface.h
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


#ifndef COMMINTERFACE_H
#define COMMINTERFACE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <CharArray.h>
#include <Component.h>
#include <Socket.h>

/**
 * @brief Communcation-Interface-Base-Class of the CommInterface template
    * specifies a standardized interface to use for integrating 
    * any hardware-interface (e.g. SoftwareSerial, OneWire...) into a Service-Interface 
    * The template ensures the correct handling of send- and receive-buffers of the interface:
    *
    * sendBuffer set to nullptr:       the Interface is ready to send another frame
    * sendBuffer not set to nullptr:   the frame, sendBuffer is pointing to, has to be send next 
    *
    * recBuffer set to nullptr:        the frame received last was written to the destination successfully, no new receive-Buffer was defined
    * recBuffer not set to nullptr:    the interface is waiting to receive a new frame to write it to the destination, recBuffer is pointing to 
    * 
    * Stores errors occurred during internal processing by using the ErrorState class. The errors can be picked-up by calling the public 
    * ErrorState functions after calling the send or receive cycles
    * 
 */
class CommInterfaceBase: public Component
{
public:
    /**
     * @brief Default-Constructor for a new Comm Interface Base object
     * 
     */
    CommInterfaceBase();

    /**
     * @brief Constructor for a new Comm Interface Base object
     * 
     */
    CommInterfaceBase(uint8_t _componentId, uint8_t _instanceId):
    Component(_componentId, _instanceId)
    {};

    /**
     * @brief Destroy the Comm-Interface Base object
     * 
     */
    ~CommInterfaceBase(){};

    /**
     * @brief Setup the Interface
     *  has to be called in Setup-function
     */
    virtual void setup_interface(){};
};

/**
    * @brief Template for generic communication-interface
    * specifies a standardized interface to use for integrating 
    * any hardware-interface (e.g. SoftwareSerial, OneWire...) into a Service-Interface 
    * The template ensures the correct handling of send- and receive-buffers of the interface:
    *
    * sendBuffer set to nullptr:       the Interface is ready to send another frame
    * sendBuffer not set to nullptr:   the frame, sendBuffer is pointing to, has to be send next 
    *
    * recBuffer set to nullptr:        the frame received last was written to the destination successfully, no new receive-Buffer was defined
    * recBuffer not set to nullptr:    the interface is waiting to receive a new frame to write it to the destination, recBuffer is pointing to 
    *     
    * Stores errors occurred during internal processing by using the ErrorState class. The errors can be picked-up by calling the public 
    * ErrorState functions after calling the send or receive cycles
    * 
    * @tparam interfaceType type of native bus-interface to setup the Comm-interface for (e.g. SoftwareSerial)
*/
template<typename interfaceType, typename routerType>                                       
class CommInterface: public CommInterfaceBase{
    private:
        /// @brief Pointer to the Router-Instance to construct the Frames from the received CharArrays and forward them to the target instances
        static routerType* frameRouter;

        /**
         * @brief Add a received Frame to the Socket for transmitting to the associated frame-router
         *  Called when connection to the Socket is tried to be established
         * 
         */
        static void onSocketConnect(){
            if(receiveBuffer.getSize() != 0){
                socket.setTxObject(&receiveBuffer, *(frameRouter->getComponentId()), *(frameRouter->getInstanceId()));
            };
        };

        /**
         * @brief Set the pointer for the Frame to be send next to the Object, the Socket has received 
         * Called after a incoming connection was established to the socket
         */
        static void onSocketReceive(){
            if(sendBuffer == nullptr){
                sendBuffer = socket.getRxObject();
            };
        };

        /**
         * @brief Initialize receive-Buffer after forwarding to another Component.
         * Called after the target-socket has acknowledged receiving the buffer-content.
         * 
         */
        static void onSocketAck(){
            receiveBuffer = CharArray();
        };


    protected:
        /// @brief CharArray-object, a received frame should be stored at
        static CharArray receiveBuffer;  

        /// @brief Pointer to CharArray-object that has to be send 
        static CharArray* sendBuffer; 

        /// @brief pointer to an instance of the native bus-interface (setup outside of BusBricks)
        static interfaceType* interface;                                                                        

        /**
         * @brief Send the frame, sendBuffer is pointing to
         * (has to be done in the derived class)
         *
         * @return true the frame, the sendBuffer was pointing to, was sent 
         * @return false the frame, the sendBuffer was pointing to, was not sent or sendBuffer nullptr (no frame to be sent)
         */
        virtual bool send() = 0;                                       
        

        /**
         * @brief Receive a Payload by receiving a Frame in a CharArray and writing the resulting Payload (representation of Frame-Object)
         * to the _receivedPayload-Buffer. 
         * (has to be implemented in the derived class)
         * 
         * @return true a new Frame was received and stored obj, receiveBuffer is pointing to 
         * @return false no new frame was received _receivedPayload still empty
         */
        virtual bool receive() = 0;                                     

    public:
        /**
         * @brief Construct a new Comm-Interface object
         * 
         * @param _interface Pointer to an instance of the native bus-interface (setup outside of BusBricks)
         * @param _frameRouter Pointer to the Router-Instance to construct the Frames from the received CharArrays and forward them to the target instances
         */
        CommInterface(interfaceType* _interface, routerType* _frameRouter, uint8_t _componentId, uint8_t _instanceId): 
        CommInterfaceBase(_componentId, _instanceId)
        {
        interface = _interface;
        frameRouter = _frameRouter;
        socket = Socket(componentId, instanceId, onSocketConnect, onSocketReceive, onSocketAck);
        }; 

        /// @brief Socket to connect CommInterface-Component to SocketRouter-Component
        static Socket socket;
};
#endif // COMMINTERFACE_H