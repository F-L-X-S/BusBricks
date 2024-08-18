/**
 * @file ServiceInterface.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SERVICEINTERFACE_H
#define SERVICEINTERFACE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <CommInterface.h>
#include <ServiceCluster.h>
#include <Frame.h>
#include <Content_stack.h>

#define STACKSIZE 2         // Size of Send- / Rec-Stack
#define DEBUG


/**
 * @brief Template for generic Service-Interface
 * Defines the handling of incoming frames from CommInterface to services stored in the associated ServiceCluster
 * and the conversion from PDU provided by Services in the ServiceCluster to frames, getting sent by the communication-interface.
 * 
 * @tparam CommInterfaceBase type of CommInterface to use, derived Class of the abstract CommInterface-Class (e.g. CommInterface_modbusRTU)
 * @tparam frameType type of the frames to be send, derived Class of the abstract Frame-Class (e.g. Frame_modbusRTU)
 */
template<typename CommInterfaceBase, typename frameType>                                       
class ServiceInterface{
    public:
        /**
         * @brief Construct a new Service Interface object
         * 
         * @param comm_interface pointer to an instance of a CommInterface-derived class-object 
         * @param services pointer to an instance of a ServiceCluster-derived class-object containing the associated services 
         */
        ServiceInterface(CommInterfaceBase* comm_interface, ServiceClusterBase* services):
                comm_interface(comm_interface), 
                services(services) {};

        /**
         * @brief Execute all relevant tasks for transferring data between CommInterface and Services:
         * Getting payloads to sent from the Services, adding received payloads to the right Services,
         * Updating communication-stacks, Executing the CommInterfaces send- and receive-cycle
         * (Has to be implemented in the derived class)
         * 
         */
        virtual void communicate() = 0;        

    protected:
        /// @brief pointer to Communication-Interface (instance of CommInterface-derived class)
        CommInterfaceBase* comm_interface;        

        /// @brief pointer to ServiceCluster, containing the Services associated with the Interface (instance of ServiceCluster-derived class)             
        ServiceClusterBase* services;                               

        /// @brief stack for received frames (instance of Content_stack-derived class, specified for type of frames, the bus is using)    
        Content_stack<frameType, STACKSIZE> recStack;     

        /// @brief stack for frames to send next (instance of Content_stack-derived class, specified for type of frames, the bus is using)        
        Content_stack<frameType, STACKSIZE> sendStack;        

        /// @brief Item to be sent next     
        String sendItem;  

        /// @brief Item received last                                          
        String recItem;                                             

        /**
         * @brief Add all PDUs provided by the services to the sendstack,
         * depending on how frametype stores the information, which service the payload is designated for
         * (Implemented in derived Class)
         * 
         */
        virtual void getPDU_from_services()=0;

        /**
         * @brief Impart all received PDUs from the receivestack to the associated services, 
         * depending on how frametype stores the information, which service the payload is designated for
         * (Implemented in derived Class)
         * 
         */
        virtual void addPDU_to_services()=0;


        /**
         * @brief start the stackProcessing of the registered services 
         * 
         */
        virtual void processServices(){
            for (size_t i = 0; i < services->getNumberOfServices(); i++)
                {
                    ServiceBase* destinationService = services->getService_byPos(i);            // Pointer to the destination-Service 
                    destinationService->stackProcessing();
                }
        }

        /**
         * @brief Add items from the sendStack to the comm_interface and execute the sendCycle of CommInterface
         * delete the sent item from the sendStack
         * 
         */
        virtual void processSendStack(){
            // Serial debugging
            #ifdef DEBUG
                Serial.println("Processing Send-Stack...");
            #endif
            // Handle sendstack
            while (!sendStack.empty())
            {
                if (comm_interface->finishedSending()){
                    Frame* frameToSend = sendStack.getElement();        // Get next item from the send-stack
                    sendItem = frameToSend->getFrame();                 // Extract Frame from Item
                    String* sendItemAdr = &sendItem;
                    comm_interface->sendNewFrame(sendItemAdr);          // Impart Frame that has to be sent next 
                    sendStack.deleteElement();                          // delete Item from send-stack
                    comm_interface->sendCycle();                        // execute sending 
                };           
            };        
        }

        /**
         * @brief Add items received by the CommInterface to the recStack and execute the CommInterface's receiveCycle to wait for new incoming frames
         * exit, if the recStack is full or the CommInterface did not received new Frames within the in receiveCycle specified timeout
         */
        virtual void processRecStack() {
            // Serial debugging
            #ifdef DEBUG
                Serial.println("Processing Receive-Stack...");
            #endif                  
            // Handle receivebuffer 
            while (comm_interface->receivedNewFrame() && !recStack.full()){
                if (recItem != ""){                                 // Item not empty 
                    frameString frameStr = recItem.c_str();         // conversion for identification as framestring in Frame-Class-Constructor
                    frameType recItemFrame(&frameStr);              // Construct Frame-Class derived Object
                    recStack.addElement(recItemFrame);              // Add the received element to the stack
                    recItem = "";                                   // Clear rec-Item 
                }
                comm_interface->getReceivedFrame(&recItem);         // Impart memory the received item has to be stored at 
                comm_interface->receiveCycle();                     // Receive new frames from comm-interface 
            }
            comm_interface->receiveCycle();                         // Receive new frames from comm-interface
        }
};
#endif // SERVICEINTERFACE_H