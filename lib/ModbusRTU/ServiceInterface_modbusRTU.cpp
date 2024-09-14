/**
 * @file ServiceInterface_modbusRTU.cpp
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

#include "ServiceInterface_modbusRTU.h"

// Construct Service-Interface 
ServiceInterface_modbusRTU::ServiceInterface_modbusRTU(ServiceClusterBase* services, CommInterface_modbusRTU* comm_interface):
ServiceInterface<CommInterface_modbusRTU, Frame_modbusRTU>(comm_interface, services){};

// Destructor for ServiceInterface_modbusRTU
ServiceInterface_modbusRTU::~ServiceInterface_modbusRTU() {
}


// Add all PDUs provided by the services to the sendstack:
// - iterate through services of the service-cluster 
// - create frame from payload provided by service (service-/payload-specific)
void ServiceInterface_modbusRTU::getPDU_from_services()
{
    for (size_t i = 0; i < services->getNumberOfServices(); i++)
    {
        if (sendStack.full()) break;                                                // exit the loop if no space left in send stack
        ServiceBase* destinationService = services->getService_byPos(i);            // Pointer to the destination-Service 
        while (destinationService->responseAvailable()){                            // skip iteration if the services response is empty 
            String servicePdu = destinationService->get_response();              // get the response-PDU provided by the service
            char deviceId = *destinationService->get_InstanceID();                  // initialize the device-id by instance-id 

            // Handle service-type specific conversions
            if (*destinationService->get_ServiceID() == static_cast<uint8_t>('m'))
            {
                MessageService* messageService = static_cast<MessageService*>(destinationService);   // cast as Message-service for specific functions
                uint8_t destId = messageService->get_destinationId();                   // Get the Destination-Device ID (depending on forwarding flag)
                deviceId = destId;
            }

            // Build Frame
            char functionCode = *(destinationService->get_ServiceID());                 // Get the Function Code
            Frame_modbusRTU frame(&servicePdu, &deviceId, &functionCode);               // Construct the modbus-frame
            sendStack.addElement(frame);                                                // Add the Frame to the Interface-send-stack
            destinationService->clearResponse();                                
         };
    }
    
};

// Add PDU from all received Frames to the dedicated Services 
void ServiceInterface_modbusRTU::addPDU_to_services()
{
    // abort, if no new PDU available
    if (recStack.empty()) return;
    // Add all received PDUs to the Services 
    while (!recStack.empty())
    {
        Frame_modbusRTU* receivedFrame = recStack.getElement();
        if(!receivedFrame->checkCRC16()){
            raiseError(crcError);
        }else{
            char ServiceID = receivedFrame->getFunctionCode();                          // Get the Modbus-RTU-Function-Code as Service-ID 
            ServiceBase* destinationService = services->getService_byID(ServiceID);     // Pointer to the destination-Service 
            if (!destinationService){
                raiseError(serviceNotFound);                                            // raise Service-not-found-error
                recStack.deleteElement();                                               // discard invalid Frame (no Service found)
                continue;
            }
            String pdu = *receivedFrame->get_content();                                       // Get the Frames payload 
            destinationService->impart_pdu(&pdu);                                       // Add a Content-Object created from PDU to the Services receive-stack 
        }
        recStack.deleteElement();                                                       // Delete the item added to services rec-stack from the interface-rec-stack
    };
};

// Execute all relevant tasks for transferring data between CommInterface and Services:
// - Get PDU from Services
// - Add PDU to Services
// - Update communication-stacks
// - Execute the CommInterfaces Comm-cycle 
void ServiceInterface_modbusRTU::communicate()
{
    // get incoming frames from the Communication-Interface 
    processRecStack();

    // interact with Services          
    addPDU_to_services();
    processServices();
    getPDU_from_services();
    
    // impart the new Frames to the Communication-Interface 
    processSendStack();
};