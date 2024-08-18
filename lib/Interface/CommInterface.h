/**
 * @file CommInterface.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
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

#include<Frame.h>

/**
 * @brief Communcation-Interface-Base-Class 
 * containing functions, that have to be re-implemented in the derived classes  of the CommInterface-template 
 * but which have to be called in other (abstract) classes (e.g. ServiceInterface)
 */
class CommInterfaceBase
{
public:
    /**
     * @brief Setup the Interface
     *  has to be called in Setup-function
     */
    void setup_interface(){};

    /**
     * @brief Add a new Frame to the send-buffer
     * 
     * @param sendFrame Pointer to String-Object with frame to be send next  
     */
    void sendNewFrame(String* sendFrame){};

    /**
     * @brief Check, if the Frame was sent and the CommInterface is ready to send the next Frame 
     * 
     * @return true Frame, that was added by sendNewFrame-function was send 
     * @return false Frame, that was added by sendNewFrame-function is not sent yet 
     */
    bool finishedSending(){};

    /**
     * @brief Define the destination, the next received Frame should be copied to ba a pointer to an empty String-Object
     * 
     * @param destFrameBuffer pointer to an empty String-Object, the next received frame should be stored in
     */
    void getReceivedFrame(String* destFrameBuffer){};

    /**
     * @brief Check, if a new Frame was received 
     * 
     * @return true a new frame was received and stored on the with getReceivedFrame specified String-Object
     * @return false no new frame was received, getReceivedFrame specified String-Object still empty
     */
    bool receivedNewFrame(){};

    /**
     * @brief Construct a new Comm Interface Base object
     * 
     */
    CommInterfaceBase(){};

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
    * @tparam interface_type type of native bus-interface to setup the Comm-interface for (e.g. SoftwareSerial)
*/
template<typename interface_type>                                       
class CommInterface: public CommInterfaceBase{
    protected:
        /// @brief pointer to an instance of the native bus-interface (setup outside of BusBricks)
        interface_type* interface;                       

        /// @brief pointer to the next frame to be send, set to nullptr if Frame was sent 
        String* sendBuffer = nullptr;                              

        /// @brief pointer to String-object, a received frame should be stored at, set to nullptr if Frame was copied to destination
        String* receiveBuffer = nullptr;                           

        // Send a Frame 
        // True, if a frame was send successfully 

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
         * @param sendFrame String-pointer to next frame to be send
         */
        void sendNewFrame(String* sendFrame){
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
         * @param destFrameBuffer pointer to String-object, the next received Frame should be stored at 
         */
        void getReceivedFrame(String* destFrameBuffer){
            receiveBuffer = destFrameBuffer;                            // set the receive-buffer-ptr to the destination, the next received Frame should be stored at 
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