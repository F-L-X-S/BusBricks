/**
 * @file main.cpp
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief This main is an example-implementation of a simple modbus-messenger to demonstrate the usage
 * of a layer-7-service in the program. 
 * The String typed in by the user via serial-interface is sent to the device with PARTNER_DEVICE_ID.
 * All received Messages and raised errors are displayed on serial-monitor.
 * Switch PARTNER_DEVICE_ID and DEVICE_ID before uploading to the Partner-device.
 * 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright * Copyright (C) 2024 Felix Schuelke
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <mockArduino.h>
#endif

#define DEBUG

#include <ServiceInterface_modbusRTU.h>
#include <CommInterface_modbusRTU.h>
#include <MessageService.h>
#include <ErrorService.h>

#define DEVICE_ID 'A'         // Modbus-RTU specific Device-ID 
#define PARTNER_DEVICE_ID 'B' // Modbus-RTU specific Device-ID of an Partner-Device

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
 MessageService msgService(DEVICE_ID); 

// instantiate  Error Service with default Service-ID "e"
ErrorService errService(DEVICE_ID); 

// register the services in a service-cluster
ServiceBase* serviceList[2] = {&msgService, &errService};         

// Create a Service-Cluster from ptr-list to the associated services 
// RAM: 20 bytes
ServiceCluster<2> services(serviceList);              

// instantiate the service-interface 
// RAM: 146 bytes (Stacksize 3)
ServiceInterface_modbusRTU serviceinterface(&services, &comminterface);         // construct from ref. to associated service-cluster and communication-interface

// Decide between Arduino-build and native-build 
#ifdef ARDUINO

void setup() {
    Serial.begin(9600);
    serialInterface.begin(BAUDRATE);
    Serial.println("Setup...");
    msgService.sendMessage(PARTNER_DEVICE_ID, "Setup...");
};

void loop() {
    String s = "";
    // wait for a user-input 
    while (Serial.available() != 0) {      // data available
        s = Serial.readString();           // read until timeout
        s.trim();                          // formatting
    };
    if (s!="") msgService.sendMessage(PARTNER_DEVICE_ID,s); // send a message to the Partner-device with the given text
    serviceinterface.communicate();         // execute the communication-cycle
 };

#else
// define main-function to prevent undefined-symbol _main in native build 
int main(){}

#endif // ARDUINO