#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <mockArduino.h>
#endif

#define DEBUG

#include <ServiceInterface_modbusRTU.h>
#include <CommInterface_modbusRTU.h>
#include <Message_service.h>
#include <Service.h>

#define DEVICE_ID 'A'         // Modbus-RTU specific Device-ID 
#define BAUDRATE 9600
#define RX 2
#define TX 3

//---------------------------- Communication-Layer -----------------------
// instantiate communication-interface
SoftwareSerial serialInterface(RX, TX);                                         // initialize a Software-Serial-Interface

// Inastantiate Communication-Interface
// RAM: 35 bytes
CommInterface_modbusRTU comminterface(&serialInterface, BAUDRATE, DEVICE_ID);   

//---------------------------- Service-Layer ----------------------------
// instantiate  Message Service with default Service-ID "m"
// RAM: 1639 bytes (Stacksize 3)
 Message_service msg_service(DEVICE_ID); 

// register the services in a service-cluster
ServiceBase* serviceList[1] = {&msg_service};         // Array of service-references

// Create a Service-Cluster from ptr-list to the associated services 
// RAM: 20 bytes
ServiceCluster<1> services(serviceList);              

// instantiate the service-interface 
// RAM: 146 bytes (Stacksize 3)
 ServiceInterface_modbusRTU serviceinterface(&services, &comminterface);         // construct from ref. to associated service-cluster and communication-interface



void setup() {
    Serial.begin(9600);
    serialInterface.begin(BAUDRATE);
    Serial.println("Setup...");
    msg_service.sendMessage('B', "Setup...");
};

void loop() {
    String s = "";
    while (Serial.available() != 0) {      // data available
        s = Serial.readString();           //read until timeout
        s.trim();  
    };
    if (s!="") msg_service.sendMessage('B',s);
    serviceinterface.communicate();
 };
