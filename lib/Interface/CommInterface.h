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

#include<CharArray.h>
#include<ErrorState.h>

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
class CommInterfaceBase: public ErrorState
{
public:
    /**
     * @brief Setup the Interface
     *  has to be called in Setup-function
     */
    virtual void setup_interface(){};

    /**
     * @brief Add a new Frame to the send-buffer
     * 
     * @param sendFrame Pointer to CharArray-Object with frame to be send next  
     */
    virtual void sendNewFrame(CharArray* sendFrame){};

    /**
     * @brief Check, if the Frame was sent and the CommInterface is ready to send the next Frame 
     * 
     * @return true Frame, that was added by sendNewFrame-function was send 
     * @return false Frame, that was added by sendNewFrame-function is not sent yet 
     */
    virtual bool finishedSending(){};

    /**
     * @brief Define the destination, the next received Frame should be copied to ba a pointer to an empty String-Object
     * 
     * @param destFrameBuffer pointer to an empty CharArray-Object, the next received frame should be stored in
     */
    virtual void getReceivedFrame(CharArray* destFrameBuffer){};

    /**
     * @brief Check, if a new Frame was received 
     * 
     * @return true a new frame was received and stored on the with getReceivedFrame specified Frame-Object
     * @return false no new frame was received, getReceivedFrame specified Frame-Object still empty
     */
    virtual bool receivedNewFrame(){};

    /**
     * @brief Construct a new Comm Interface Base object
     * 
     */
    CommInterfaceBase(): ErrorState(){};

    /**
     * @brief Destroy the Comm-Interface Base object
     * 
     */
    ~CommInterfaceBase(){};
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
    * @tparam interface_type type of native bus-interface to setup the Comm-interface for (e.g. SoftwareSerial)
*/
template<typename interface_type>                                       
class CommInterface: public CommInterfaceBase{
    protected:
        /// @brief pointer to an instance of the native bus-interface (setup outside of BusBricks)
        interface_type* interface;                       

        /// @brief pointer to the next frame to be send, set to nullptr if Frame was sent 
        CharArray* sendBuffer = nullptr;                              

        /// @brief pointer to CharArray-object, a received frame should be stored at, set to nullptr if Frame was copied to destination
        CharArray* receiveBuffer = nullptr;                           

        /**
         * @brief Send the frame, sendBuffer is pointing to
         * (has to be done in the derived class)
         *
         * @return true the frame, the sendBuffer was pointing to, was sent 
         * @return false the frame, the sendBuffer was pointing to, was not sent or sendBuffer nullptr (no frame to be sent)
         */
        virtual bool send() = 0;                                       
        

        /**
         * @brief Receive a Frame 
         * (has to be done in the derived class)
         * 
         * @return true a new Frame was received and stored at the String-obj, receiveBuffer is pointing to 
         * @return false no new frame was received receiveBuffer still empty
         */
        virtual bool receive() = 0;                                     

    public:
        /**
         * @brief Construct a new Comm-Interface object
         * 
         * @param interface pointer to an instance of the native bus-interface (setup outside of BusBricks)
         * @param baudrate bits per second
         */
        CommInterface(interface_type* interface, uint32_t baudrate): interface(interface){}; 

        /**
         * @brief Specify the next frame to be sent by the interface, 
         * no overwrite, if the sendBuffer is already pointing to another frame
         * 
         * @param sendFrame pointer to next Frame-object to be send
         */
        void sendNewFrame(CharArray* sendFrame){
            if (sendBuffer == nullptr)
            {
                sendBuffer = sendFrame;                                 // set the send-buffer-ptr to the Frame, that has to be sent 
            }
        };

        /**
         * @brief Check, if the Frame was sent and the CommInterface is ready to send the next Frame 
         * Interface is ready to send a new frame, if the sendBuffer is nullpointer 
         * 
         * @return true ready to accept a new frame to send next with sendNewFrame-function
         * @return false frame, sendBuffer is pointing to is not sent yet
         * not ready to to accept a new frame in sendNewFrame-function
         */
        bool finishedSending(){
            return sendBuffer==nullptr;
        };

        /**
         * @brief Define the destination, the next received Frame should be stored at 
         * 
         * @param externalRecBuffer pointer to CharArray-object, the next received Frame should be stored at 
         */
        void getReceivedFrame(CharArray* externalRecBuffer){
            receiveBuffer = externalRecBuffer;                            // set the receive-buffer-ptr to the destination, the next received Frame should be stored at 
        };

        /**
         * @brief Check, if a new Frame was received and stored at the in getReceivedFrame specified location
         * the interface is waiting waiting for getReceivedFrame-call to specify the location, the next received frame should be stored at
         * 
         * @return true a new frame was received and stored, waiting for getReceivedFrame-call to specify receiveBuffer
         * @return false receiveBuffer is specified, but still empty
         */
        bool receivedNewFrame(){
            return receiveBuffer==nullptr;
        };

        /**
         * @brief Execution of receive-cycle, set the receiveBuffer to nullptr after a new frame was received 
         * 
         */
        virtual void receiveCycle(){                                     
            // Receiving
            if (receive()) {
                receiveBuffer = nullptr;                                
            };
        };
        
        /**
         * @brief Execution of send-cycle, set the sendBuffer to nullptr, after the frame, sendBuffer was pointing at, was send 
         * 
         */
        virtual void sendCycle(){   
            // Sending
            if (send()){
                sendBuffer = nullptr;                                   
            }; 
        };
};
#endif // COMMINTERFACE_H