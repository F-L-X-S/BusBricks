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