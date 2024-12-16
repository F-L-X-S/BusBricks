/**
 * @file ServiceInterface.h
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
#include <ErrorService.h>
#include <ErrorState.h>

#define STACKSIZE 2             // Size of Send- / Rec-Stack
#define ERRORSERVICE_ID 'e'     // Service-ID of the configured Error-Service in Servicecluster
//#define DEBUG


/**
 * @brief Template for generic Service-Interface
 * Defines the handling of incoming frames from CommInterface to services stored in the associated ServiceCluster
 * and the conversion from PDU provided by Services in the ServiceCluster to frames, getting sent by the communication-interface.
 * The ServiceInterface dedicates memory to the CommInterface by iterating through it's stacks.
 * 
 * @tparam CommInterfaceBase type of CommInterface to use, derived Class of the abstract CommInterface-Class (e.g. CommInterface_modbusRTU)
 * @tparam frameType type of the frames to be send, derived Class of the abstract Frame-Class (e.g. Frame_modbusRTU)
 */
template<typename CommInterfaceBase, typename frameType>                                       
class ServiceInterface: public ErrorState{
    public:
        /**
         * @brief Construct a new Service Interface object
         * 
         * @param comm_interface pointer to an instance of a CommInterface-derived class-object 
         * @param services pointer to an instance of a ServiceCluster-derived class-object containing the associated services 
         */
        ServiceInterface(CommInterfaceBase* comm_interface, ServiceClusterBase* services):
                ErrorState(),
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
        CharArray sendItem;  

        /// @brief Item received last                                          
        CharArray recItem;                                             

        /**
         * @brief Add all PDUs provided by the services to the sendstack,
         * depending on how frametype stores the information, which service the payload is designated for
         * (Implemented in derived Class)
         * 
         */
        virtual void getPDU_from_services()=0;

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
         * @brief Check if an ErrorService is registered in the service cluster under the defined 
         * ERRORSERVICE_ID of the Service Interface and call raiseError for that Service. 
         * If no ErrorService is registered, the errorState is stored regularly in the ErrorState class.
         * 
         * @param code The error code of the error to be raised.
         */
        void raiseError(errorCodes code) {
            ServiceBase* service = services->getService_byID(ERRORSERVICE_ID);
            if (service) {
                ErrorService* errService = static_cast<ErrorService*>(service);
                errService->raiseError(code);
            }else{
                ErrorState::raiseError(code);
            }
        };

        /**
         * @brief Send all Frames, stored in the sendStack. 
         * As long as the sendStack is not empty, the function is getting pointers to the representation of the Frame on the bottom of the stack.
         * The pointer to the string-formatted representation is imparted to the CommInterface as teh next frame to be sent by calling sendNewFrame.
         * Then the CommInterface's sendCycle get's executed.
         * Exit, after the sendStack is empty.
         * 
         * The ErrorStates of the CommInterface are transferred to the ServiceInterface ErrorState.
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
                    Frame* frameToSend = sendStack.getElement();                        // Get next item from the send-stack
                    sendItem = *frameToSend->get_representation();                      // buffer Frame as char-array
                    comm_interface->sendNewFrame(&sendItem);                            // Impart Frame that has to be sent next to the CommInterface
                    sendStack.deleteElement();                                          // delete Item from send-stack
                    comm_interface->sendCycle();                                        // execute sending 
                }else{
                    comm_interface->sendCycle();                                        // execute sending 
                };
                
                // check the commInterfaces ErrorState 
                errorCodes commInterfaceErrorState = comm_interface->getErrorState();
                if (commInterfaceErrorState!=noError) raiseError(commInterfaceErrorState);
                comm_interface->clearErrorState();            
            };        
        }

        /**
         * @brief Add PDUs from all received frames to the corresponding services.
         * 
         * This function processes all frames available in the receive stack (`recStack`). 
         * For each frame, it checks the CRC-16 checksum and, if valid, adds the PDU to the appropriate service's receive stack. 
         * If the frame's CRC check fails, it raises a CRC error. If no matching service is found for the frame's function code, 
         * it raises a service-not-found error and discards the frame.
         * 
         * @details
         * - If the receive stack is empty, the function returns immediately.
         * - Frames with no matching service are discarded, and a service-not-found error is raised.
         * - Valid frames are added to the corresponding service's receive stack.
         * 
         * @note The function continues processing until the receive stack is empty.
         */
        virtual void addPDU_to_services()
        {
            // Add all received PDUs to the Services 
            while (!recStack.empty())
            {
                // Try to add Frame from rec-stack to Service
                Frame* receivedFrame = recStack.getElement();     
                errorCodes ServicesErrorState = services->impartPdu(receivedFrame);

                // handle the Service-Clusters error-state
                if (ServicesErrorState != noError) raiseError(ServicesErrorState);

                // clear Error-state after handling error
                services->clearErrorState();

                // Skip Discard and leave the rec-stack-processing
                if (ServicesErrorState == overflow) break; 

                // discard frame
                recStack.deleteElement();
            };
        };


        /**
         * @brief Add items received by the CommInterface to the recStack and execute the CommInterface's receiveCycle to wait for new incoming frames.
         * Exit, if the recStack is full or the CommInterface did not received new Frames within the in receiveCycle specified timeout.
         * 
         * If the CommInterface received a new frame (that means, it's recBuffer is set to nullptr), the function creates the 
         * Frame-instance (specified by frameType) from it's recItem and adds it to the recStack. 
         * (If the creation of the Frame-instance fails (isValid returned false), the Framing-Error is raised)
         * Then it initializes the recItem and imparts it's reference to the comm_interface by calling getReceivedFrame. 
         * The CommInterface is now storing the next Frame received at this address. 
         * After specifying the destination for received frames for ComInterface, the receiveCycle is executed again, to wait for incoming Frames.
         * 
         * The ErrorStates of the CommInterface are transferred to the ServiceInterface ErrorState.
         */
        virtual void processRecStack() {
            // Serial debugging
            #ifdef DEBUG
                Serial.println("Processing Receive-Stack...");
            #endif      
            // exit if no space on rec-stack
            if (recStack.full()) return; 

            // Handle receivebuffer 
            if (comm_interface->receivedNewFrame()){               
                if (recItem.getSize() != 0){                        // Item not empty 
                    frameType recItemFrame(&recItem);               // Construct Frame-Class derived Object (nullptr if failed)
                    if (recItemFrame.isValid()){
                        recStack.addElement(recItemFrame);          // Add the received element to the stack
                        }else{
                            raiseError(framingError);}              // frame-construction failed 
                    recItem = CharArray();                          // Clear rec-Item                               
                }

                // check the commInterfaces ErrorState 
                errorCodes commInterfaceErrorState = comm_interface->getErrorState();
                if (commInterfaceErrorState!=noError) raiseError(commInterfaceErrorState);
                comm_interface->clearErrorState(); 

                // Impart memory the received item has to be stored at 
                comm_interface->getReceivedFrame(&recItem);         
            }
            comm_interface->receiveCycle();                         // Receive new frames from comm-interface

            // check the commInterfaces ErrorState 
            errorCodes commInterfaceErrorState = comm_interface->getErrorState();
            if (commInterfaceErrorState!=noError) raiseError(commInterfaceErrorState);
            comm_interface->clearErrorState(); 
        }
};
#endif // SERVICEINTERFACE_H