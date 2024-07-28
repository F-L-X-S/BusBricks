#include <mockArduino.h>
#include <mockSoftwareSerial.h>

#include <ServiceInterface_modbusRTU.h>
#include <CommInterface_modbusRTU.h>
#include <Message_service.h>
#include <Service.h>

#define DEVICE_ID '1'           // Modbus-RTU specific Device-ID
#define INSTANCE_ID_A 'm'       // Service-identifier 
#define INSTANCE_ID_B 'n'       // Service-identifier

int main(){

    //---------------------------- Service-Layer ----------------------------
    // instantiate services
    Message_service msg_service_a(INSTANCE_ID_A);   
    Message_service msg_service_b(INSTANCE_ID_B);   

    // register the services in a service-cluster
    ServiceBase* serviceList[2] = {&msg_service_b, &msg_service_b};         // Array of service-references
    ServiceCluster<2> services(serviceList);                                // Create a Service-Cluster from ptr-list to the associated services 


    //---------------------------- Communication-Layer -----------------------
    // instantiate communication-interface
    uint8_t rxPin = 1;
    uint8_t txPin = 2;
    arduinoMocking::SoftwareSerial sim_serial(rxPin, txPin);              // initialize a mocked Software-Serial 
    CommInterface_modbusRTU comminterface(sim_serial, 9600, DEVICE_ID);   // Softwareserial, baudrate, Modbus-device-id 

    // instantiate the service-interface 
    ServiceInterface_modbusRTU serviceinterface(&services, &comminterface); // construct from ref. to associated service-cluster and communication-interface
    
    //---------------------------- Simulate Arduino Setup -------------
    comminterface.setup_interface();                                      // Call setup-function of Comm Interface

    //---------------------------- Simulate Arduino Loop -------------
    // Devicesettings 
    char senderDeviceId = 0x2;
    char functionCodeA = INSTANCE_ID_A;
    char functionCodeB = INSTANCE_ID_B;

    // incoming frame example
    pduString incomingFramePdu = "Incoming Frame No. 1";
    Frame_modbusRTU incomingFrame(&incomingFramePdu, &senderDeviceId, &functionCodeA);
    String frame = *incomingFrame.get_representation();
    sim_serial.simulateInput(frame);
    serviceinterface.communicate();
    sim_serial.read();
    sim_serial.flush();
    
};