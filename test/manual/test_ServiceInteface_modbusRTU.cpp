#include <mockArduino.h>
#include <mockSoftwareSerial.h>

#include <ServiceInterface_modbusRTU.h>
#include <CommInterface_modbusRTU.h>
#include <Message_service.h>
#include <Service.h>

#define DEVICE_ID_ONE 'A'         // Modbus-RTU specific Device-ID of the simulated device 
#define DEVICE_ID_TWO 'B'         // Modbus-RTU specific Device-ID of the simulated device 
#define INSTANCE_ID_A 'm'       // Service-identifier of Service-Instance one
#define INSTANCE_ID_B 'n'       // Service-identifier of Service-Instance two

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
    CommInterface_modbusRTU comminterface(&sim_serial, 9600, DEVICE_ID_ONE);   // Softwareserial, baudrate, Modbus-device-id 

    // instantiate the service-interface 
    ServiceInterface_modbusRTU serviceinterface(&services, &comminterface); // construct from ref. to associated service-cluster and communication-interface
    
    //---------------------------- Simulate Arduino Setup -------------
    comminterface.setup_interface();                                      // Call setup-function of Comm Interface

    //---------------------------- Simulate Arduino Loop -------------
    // Devicesettings 
    char DeviceIdOne = DEVICE_ID_ONE;
    char DeviceIdTwo = DEVICE_ID_TWO;
    char functionCodeA = INSTANCE_ID_A;
    char functionCodeB = INSTANCE_ID_B;

    // simulate an incoming frame from device two to device one for service m (Modbus-function-code = service-id)
    // simulated PDU 
    Message_content_t content_msgNoOne;
    content_msgNoOne.sender_id = DeviceIdTwo;
    content_msgNoOne.receiver_id = DeviceIdOne;
    strcpy(content_msgNoOne.msg_text, "Incoming Message No. 1");   
    Message msgNoOne(&content_msgNoOne);

    // PDU to Modbus-Frame
    Frame_modbusRTU frameNoOne(msgNoOne.get_representation(), &DeviceIdOne, &functionCodeA);
    String frameNoOne_rep = *frameNoOne.get_representation();

    // simulate an incoming frame fro mocked serial-interface 
    sim_serial.simulateInput(frameNoOne_rep);

    // execute the communication-cycle 
    serviceinterface.communicate();

    // get the output of the mocked interface
    sim_serial.read();
    sim_serial.flush();
    
};