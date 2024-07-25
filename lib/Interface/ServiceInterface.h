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
class ServiceInterface{
    public:
        // Constructor for Service-Interface 
        ServiceInterface(comm_interface_type* comm_interface, ServiceClusterBase* services):
                comm_interface(comm_interface), 
                services(services) {};

        // Execute all relevant tasks for transferring data between CommInterface and Services:
        // - Get PDU from Services
        // - Add PDU to Services
        // - Update communication-stacks
        // - Execute the CommInterfaces Comm-cycle 
        //
        // Has to be implemented in the derived class
        virtual void communicate() = 0;        

    protected:
        commInterface_type* comm_interface;                              // Communication-Interface 
        ServiceClusterBase* services;                                    // ServiceCluster containing the Services associated with the Interface 
        Content_stack<frameType, STACKSIZE> recStack;                   // stack for received frames
        Content_stack<frameType, STACKSIZE> sendStack;                  // stack for frames to send next
        frameString sendItem;                                             // Item to be sent next
        frameString recItem;                                              // Item received last

        // Add all PDUs provided by the services to the sendstack
        // Implemented in derived Class, depending on frametype
        virtual void getPDU_from_services()=0;

        // Impart all received PDUs from the receivestack to the associated services 
        // Implemented in derived Class, depending on frametype
        virtual void addPDU_to_services()=0;

        // Update stacks interacting with Communication-Interface:
        // Add the last item from the sendstack to the Comminterface, delete it after sending
        // Add items received by the CommInterface to the Receivestack
        void updateCommStacks(){
            // Handle sendstack
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
        };
};
#endif // INTERFACE_H