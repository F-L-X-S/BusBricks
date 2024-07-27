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
        CommInterface_modbusRTU* comm_interface;                          // specified Communication-Interface 

        // Add all PDUs provided by the services to the sendstack
        void getPDU_from_services() override;

        // Impart all received PDUs from the receivestack to the associated services 
        void addPDU_to_services() override;
};

#endif // SERVICEINTERFACE_MODBUSRTU