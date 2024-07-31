#ifndef SERVICEINTERFACE_MODBUSRTU
#define SERVICEINTERFACE_MODBUSRTU
#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <ServiceInterface.h>
#include <CommInterface_modbusRTU.h>
#include <Frame_modbusRTU.h>
#include <Message_service.h>

// Service-Interface-Instance for Modbus-RTU:
// Incoming information:
// Impart incoming frames from the Communication-Interface 
// to the designated service of the service-cluster 
//
// Outgoing information: 
// Build outgoing modbus-RTU-frames from the payload provided 
// by the services and impart them to the Communication-Interface
//
class ServiceInterface_modbusRTU: public ServiceInterface<CommInterface_modbusRTU, Frame_modbusRTU>{
    public:
        // Construct Service-Interface 
        ServiceInterface_modbusRTU(ServiceClusterBase* services, CommInterface_modbusRTU* comm_interface);

        // Destroy Service-Interface 
        ~ServiceInterface_modbusRTU();

        // Execute all relevant tasks for transferring data between CommInterface and Services:
        // - Get PDU from Services
        // - Add PDU to Services
        // - Update communication-stacks
        // - Execute the CommInterfaces Comm-cycle 
        void communicate() override;

    private:
        // Add all PDUs provided by the services to the sendstack
        void getPDU_from_services() override;

        // Impart all received PDUs from the receivestack to the associated services 
        void addPDU_to_services() override;
};

#endif // SERVICEINTERFACE_MODBUSRTU