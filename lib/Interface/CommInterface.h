#ifndef COMMINTERFACE_H
#define COMMINTERFACE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include<Frame.h>

// Communcation-Interface-Base-Class 
class CommInterfaceBase
{
public:
    // Setup the Interface, has to be called in Setup-function
    void setup_interface(){};

    // Add a new Frame to the send-buffer
    void sendNewFrame(String* sendFrame){};

    // Check, if the Frame was sent and the CommInterface is ready to send the next Frame 
    bool finishedSending(){};

    // Define the destination, the next received Frame should be copied to 
    void getReceivedFrame(String* destFrameBuffer){};

    // Check, if a new Frame was received 
    bool receivedNewFrame(){};

    // Execute the Communication-Cycle
    void execCommunicationCycle(){};

    CommInterfaceBase(/* args */){};
    ~CommInterfaceBase(){};
};


// Template for generic communication-interface
// specifies a standardized interface to use for integrating 
// any hardware-interface (e.g. SoftwareSerial, OneWire...)into a Service Interface 
// The template ensures the correct handling of send- and receive-buffers of the interface:
//
// sendBuffer set to nullptr:       the Interface is ready to send another frame
// sendBuffer not set to nullptr:   the frame, sendBuffer is pointing to, has to be send next 
//
// recBuffer set to nullptr:        the frame received last was written to the destination successfully, no new receive-Buffer was defined
// recBuffer not set to nullptr:    the interface is waiting to receive a new frame to write ist to the destination, recBuffer is pointing to 

template<typename interface_type>                                       
class CommInterface: public CommInterfaceBase{
    protected:
        interface_type* interface;
        String* sendBuffer = nullptr;                              // set to nullptr if Frame was sent 
        String* receiveBuffer = nullptr;                           // set to nullptr if Frame was copied to destination

        // Send a Frame 
        // True, if a frame was send successfully 
        virtual bool send() = 0;                                        // implementation of sending a Frame has to be done in the derived class
        

        // Receive a Frame 
        // True, if a new Frame was received
        virtual bool receive() = 0;                                     // implementation of receiving a Frame has to be done in the derived class

        // Execution-order for a single send-receive-cycle
        virtual void communicationCycle(){                                     
            // Receiving
            if (receive()) {
                receiveBuffer = nullptr;                                // set the receive-buffer to nullptr after a new frame was received 
            };

            // Sending
            if (send()){
                sendBuffer = nullptr;                                   // set the send-buffer to nullptr after the frame, pointed to, was send 
            }; 
        };

    public:
        // Construct Communication-Interface 
        CommInterface(interface_type* interface, uint32_t baudrate): interface(interface){}; 

        // Add a new Frame to the send-buffer
        void sendNewFrame(String* sendFrame){
            if (sendBuffer == nullptr)
            {
                sendBuffer = sendFrame;                                 // set the send-buffer-ptr to the Frame, that has to be sent 
            }
        };

        // Check, if the Frame was sent and the CommInterface is ready to send the next Frame 
        bool finishedSending(){
            return sendBuffer==nullptr;
        };

        // Define the destination, the next received Frame should be copied to 
        void getReceivedFrame(String* destFrameBuffer){
            receiveBuffer = destFrameBuffer;                            // set the receive-buffer-ptr to the destination, the next received Frame should be stored at 
        };

        // Check, if a new Frame was received 
        bool receivedNewFrame(){
            return receiveBuffer==nullptr;
        };

        // Execute the Communication-Cycle
        void execCommunicationCycle(){
            communicationCycle();
        }
};
#endif // COMMINTERFACE_H