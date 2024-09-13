/**
 * @file test_ServiceInteface_modbusRTU.cpp
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
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

#include <mockArduino.h>
#include <mockSoftwareSerial.h>

#include <ServiceInterface_modbusRTU.h>
#include <CommInterface_modbusRTU.h>
#include <MessageService.h>
#include <Service.h>
#include <ErrorService.h>

#define DEVICE_ID_ONE 'A'         // Modbus-RTU specific Device-ID of the simulated device 
#define DEVICE_ID_TWO 'B'         // Modbus-RTU specific Device-ID of the simulated device 


    // PDU to Modbus-Frame
String getModbusFrame(char functionCode, char DeviceId, String payload){
    Frame_modbusRTU frame(&payload, &DeviceId, &functionCode);
    return *frame.get_representation();
};

int main(){

    //---------------------------- Service-Layer ----------------------------
    // instantiate  Message Service with default Service-ID "m"
    MessageService msg_service_a(DEVICE_ID_ONE); 

    // instantiate  Message Service with custom Service-ID "n"
    MessageService msg_service_b(DEVICE_ID_ONE, 'n');   

    // instatiate Error-Service with default Service-ID "e"
    ErrorService errService(DEVICE_ID_ONE);

    // register the services in a service-cluster
    ServiceBase* serviceList[3] = {&msg_service_a, &msg_service_b, &errService};            // Array of service-references
    ServiceCluster<3> services(serviceList);                                                // Create a Service-Cluster from ptr-list to the associated services 


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
    content_msgNoOne.msg_text = "Hello";   
    Message msgNoOne(&content_msgNoOne);

    // PDU to Modbus-Frame
    String frameNoOne_rep = getModbusFrame('m', DEVICE_ID_ONE, *msgNoOne.get_representation());

    // simulate an incoming frame from mocked serial-interface 
    sim_serial.simulateInput(frameNoOne_rep);

    // Add an additional new message to be sent by the service "n"
    msg_service_b.sendMessage(DEVICE_ID_TWO, "New Message for Device B");

    // execute the communication-cycle and print the Output from serial Bus (cout interface)
    // Output should contain two ACK for Service-ID "m"
    serviceinterface.communicate();         // receive incoming frame

    // two empty cycles, not outputs
    serviceinterface.communicate();
    serviceinterface.communicate();

    // simulate another incoming frame from mocked serial-interface 
    sim_serial.simulateInput(frameNoOne_rep);
    serviceinterface.communicate();         // read bus
    serviceinterface.communicate();         // flush answer 

    // simulate malformed frame
    String malformedFrame = "";
    malformedFrame += DEVICE_ID_ONE;
    malformedFrame += "m";
    sim_serial.simulateInput(malformedFrame);
    serviceinterface.communicate();
    serviceinterface.communicate();

    // simulate incoming Error
    String incomingError = getModbusFrame('e', DEVICE_ID_ONE, "B:ERROR1");
    sim_serial.simulateInput(incomingError);
    serviceinterface.communicate();
    serviceinterface.communicate();

    // simulate Service-not-found Error
    String unknownServiceMessage = getModbusFrame('s', DEVICE_ID_ONE, "message for unknown service");
    sim_serial.simulateInput(unknownServiceMessage);
    serviceinterface.communicate();
    serviceinterface.communicate();
    
};