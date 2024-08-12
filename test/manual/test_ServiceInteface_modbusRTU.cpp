#include <mockArduino.h>
#include <mockSoftwareSerial.h>

#include <ServiceInterface_modbusRTU.h>
#include <CommInterface_modbusRTU.h>
#include <Message_service.h>
#include <Service.h>

#define DEVICE_ID_ONE 'A'         // Modbus-RTU specific Device-ID of the simulated device 
#define DEVICE_ID_TWO 'B'         // Modbus-RTU specific Device-ID of the simulated device 

int main(){

    //---------------------------- Service-Layer ----------------------------
    // instantiate  Message Service with default Service-ID "m"
    Message_service msg_service_a(DEVICE_ID_ONE); 

    // instantiate  Message Service with custom Service-ID "n"
    Message_service msg_service_b(DEVICE_ID_ONE, 'n');   

    // register the services in a service-cluster
    ServiceBase* serviceList[2] = {&msg_service_a, &msg_service_b};         // Array of service-references
    ServiceCluster<2> services(serviceList);                                // Create a Service-Cluster from ptr-list to the associated services 


    //---------------------------- Communication-Layer -----------------------
    // instantiate communication-interface
    uint8_t rxPin = 1;
    uint8_t txPin = 2;
    arduinoMocking::SoftwareSerial sim_serial(rxPin, txPin);              // initialize a mocked Software-Serial 
    CommInterface_modbusRTU comminterface(&sim_serial, 9600, DEVICE_ID_ONE);   // Softwareserial, baudrate, Modbus-device-id 

    // instantiate the service-interface 
    ServiceInterface_modbusRTU serviceinterface(&services, &comminterface); // construct from ref. to associated service-cluster and communication-interface
    
    //---------------------------- Simulate Arduino Setup -------------
    comminterface.setup_interface();                                      // Call setup-function of Comm Interface

    //---------------------------- Simulate Arduino Loop -------------
    // Devicesettings 
    char DeviceIdOne = DEVICE_ID_ONE;
    char DeviceIdTwo = DEVICE_ID_TWO;

    // simulate an incoming frame from device two to device one for service m (Modbus-function-code = service-id)
    // simulated PDU 
    Message_content_t content_msgNoOne;
    content_msgNoOne.sender_id = DeviceIdTwo;
    content_msgNoOne.receiver_id = DeviceIdOne;
    content_msgNoOne.msg_text = "Incoming Message No. 1";   
    Message msgNoOne(&content_msgNoOne);

    // PDU to Modbus-Frame
    char functionCode = 'm';
    Frame_modbusRTU frameNoOne(msgNoOne.get_representation(), &DeviceIdOne, &functionCode);
    String frameNoOne_rep = *frameNoOne.get_representation();

    // simulate an incoming frame from mocked serial-interface 
    sim_serial.simulateInput(frameNoOne_rep);

    // Add an additional new message to be sent by the service "n"
    msg_service_b.sendMessage(DEVICE_ID_TWO, "New Message for Device B");

    // execute the communication-cycle and print the Output from serial Bus (cout interface)
    // Output should contain two ACK for Service-ID "m"
    serviceinterface.communicate();
    serviceinterface.communicate();
    serviceinterface.communicate();
    serviceinterface.communicate();
    serviceinterface.communicate();
    
    // simulate another incoming frame from mocked serial-interface 
    sim_serial.simulateInput(frameNoOne_rep);
    serviceinterface.communicate();
    
};