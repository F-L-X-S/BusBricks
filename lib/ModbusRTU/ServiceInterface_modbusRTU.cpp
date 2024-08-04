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
        if (!destinationService->responseAvailable()) continue;                     // skip iteration if the services response is empty 
        pduString servicePdu = destinationService->get_response();                  // get the response-PDU provided by the service
        char deviceId = *destinationService->get_InstanceID();                      // initialize the device-id by instance-id 

        // Handle service-type specific conversions
        if (*destinationService->get_ServiceID() == static_cast<uint8_t>('m'))
        {
            Message_service* messageService = static_cast<Message_service*>(destinationService);   // cast as Message-service for specific functions
            uint8_t destId = messageService->get_destinationId();                   // Get the Destination-Device ID (depending on forwarding flag)
            deviceId = destId;
        }

        // Build Frame
        char functionCode = *(destinationService->get_ServiceID());                 // Get the Function Code
        Frame_modbusRTU frame(&servicePdu, &deviceId, &functionCode);               // Construct the modbus-frame
        sendStack.addElement(frame);                                                // Add the Frame to the Interface-send-stack
    }
    
};

// Add all PDUs provided by the services to the sendstack:
void ServiceInterface_modbusRTU::addPDU_to_services()
{
    // abort, if no new PDU available
    if (recStack.empty()) return;
    // Add all received PDUs to the Services 
    while (!recStack.empty())
    {
        Frame_modbusRTU* receivedFrame = recStack.getElement();
        char ServiceID = receivedFrame->getFunctionCode();                          // Get the Modbus-RTU-Function-Code as Service-ID 
        ServiceBase* destinationService = services->getService_byID(ServiceID);     // Pointer to the destination-Service 
        if (!destinationService){
            recStack.deleteElement();                                               // discard invalid Frame (no Service found)
            continue;
        }
        String pdu = receivedFrame->getPDU();                                       // Get the Frames payload 
        destinationService->impart_pdu(&pdu);                                       // Add a Content-Object created from PDU to the Services receive-stack 
        recStack.deleteElement();                                                   // Delete the item added to services rec-stack from the interface-rec-stack
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
    
    // impart the new Frame sto the Communication-Interface 
    processSendStack();
};