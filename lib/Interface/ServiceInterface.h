#ifndef INTERFACE_H
#define INTERFACE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    using namespace std;
#endif

#include <CommInterface.h>
#include <ServiceCluster.h>
#include <Frame.h>
#include <Content_stack.h>

#define STACKSIZE 3         // Size of Send- / Rec-Stack

// Template for generic Service-Interface
// The derived class defines the handling of incoming frames from communication-interface to the services (associated by service-id)
// and the conversion from PDU provided by Services to frames getting sent by the communication-interface
// Template-parameters: 
//  -type of the Communication-Interface (e.g. SoftwareSerial, Wire...)
//  -type of the frames to be send as a derived Class of the Frame-Class (e.g. Frame_modbusRTU)
template<typename commInterface_type, typename frameType>                                       
class Interface{
    private:
        CommInterface<commInterface_type> comm_interface;               // Communication-Interface 
        ServiceClusterBase services;                                    // ServiceCluster containing the Services associated with the Interface 
        Content_stack<frameType, STACKSIZE> recStack;                   // stack for received frames
        Content_stack<frameType, STACKSIZE> sendStack;                  // stack for frames to send next
        frameType sendItem;                                             // Item to be sent next
        frameType recItem;                                              // Item received last

    protected:
        // Add all PDUs provided by the services to the sendstack
        // Implemented in derived Class, depending on frametype
        virtual void getPDU_from_services()=0;

        // Impart all received PDUs from the receivestack to the associated services 
        // Implemented in derived Class, depending on frametype
        virtual void addPDU_to_services()=0;

    public:
        Interface(comm_interface_type communication_interface, ServiceCluster* services):
                comm_interface(communication_interface), 
                services(services) {} 

        void communicate(){
            // Execute the Communication-interfaces Send-Rec-Cycle
            comm_interface.execCommunicationCycle();

            // Handle sendbuffer
            if (comm_interface.finishedSending()){
                if (sendItem == sendStack.getElement())
                {
                    sendStack.deleteElement();                      // Delete the sent element from stack (if it matches the last one sent)
                }
                if (!sendStack.empty())
                {
                    sendItem = sendStack.getElement();              // Get the next Element to be sent 
                    comm_interface.sendNewFrame(&sendItem);         // Impart Frame that has to be sent next 
                };
            };

            // Handle receivebuffer
            if (comm_interface.receivedNewFrame())
            {
                if (recItem != frameType()){                        // Item not empty 
                    recStack.addElement(recItem);                   // Add the received element to the stack 
                }
                comm_interface.getReceivedFrame(&recItem);          // Impart memory the received item has to be stored at 
            }
            
            // Add all PDUs from Rec-stack to the services
            addPDU_to_services();

            // Refill the sendstack with PDUs from the services 
            getPDU_from_services();
        };
};
#endif // INTERFACE_H