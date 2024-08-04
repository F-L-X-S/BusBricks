#include <Arduino.h>
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
CommInterface_modbusRTU comminterface(&serialInterface, BAUDRATE, DEVICE_ID);   // Softwareserial, baudrate, Modbus-device-id 

//---------------------------- Service-Layer ----------------------------
// instantiate  Message Service with default Service-ID "m"
Message_service msg_service(DEVICE_ID); 

// register the services in a service-cluster
ServiceBase* serviceList[1] = {&msg_service};         // Array of service-references
ServiceCluster<1> services(serviceList);              // Create a Service-Cluster from ptr-list to the associated services 

// instantiate the service-interface 
ServiceInterface_modbusRTU serviceinterface(&services, &comminterface);         // construct from ref. to associated service-cluster and communication-interface



void setup() {
    Serial.begin(9600);
    Serial.println("Setup...");
};

void loop() {
    Serial.println("Alive!");
    delay(1000);   
 };
