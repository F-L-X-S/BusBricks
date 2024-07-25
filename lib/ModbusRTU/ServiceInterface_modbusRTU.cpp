#include "ServiceInterface_modbusRTU.h"

// Construct Service-Interface 
ServiceInterface_modbusRTU::ServiceInterface_modbusRTU(ServiceClusterBase* services, CommInterface_modbusRTU* comm_interface):
ServiceInterface<CommInterface_modbusRTU, Frame_modbusRTU>(comm_interface, services){};


// Add all PDUs provided by the services to the sendstack
void ServiceInterface_modbusRTU::getPDU_from_services()
{

};

// Add all PDUs provided by the services to the sendstack
void ServiceInterface_modbusRTU::addPDU_to_services()
{
    // Add all received PDUs to the Services 
    while (!recStack.empty())
    {
        Frame_modbusRTU receivedFrame = recStack.getElement();
        char ServiceID = receivedFrame.getFunctionCode();                             // Get the Modbus-RTU-Function-Code as Service-ID 
        ServiceBase* destinationService = services->getService_byID(ServiceID);       // Pointer to the destination-Service 
        std::string pdu = receivedFrame.getPDU();                                     // Get the Frames payload 
        destinationService->impart_pdu(&pdu);                                         // Add a Content-Object created from PDU to the Services receive-stack 
    };
};


// Execute all relevant tasks for transferring data between CommInterface and Services:
// - Get PDU from Services
// - Add PDU to Services
// - Update communication-stacks
// - Execute the CommInterfaces Comm-cycle 
void ServiceInterface_modbusRTU::communicate()
{
    // interact with Comm-interface
    updateCommStacks();   

    // interact with Services          
    addPDU_to_services();
    getPDU_from_services();
};