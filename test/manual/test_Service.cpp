/**
 * @file test_Service.cpp
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


#include "Service.h"
#include "Message.h"

#define STACKSIZE 3
#define SERVICEID 0x6D // ASCII: "m"
#define INSTANCE_ID 0xF

int main() {
    // Instantiate  simple Service for "Message"-content  
    Service<Message, STACKSIZE> message_service(SERVICEID, INSTANCE_ID);
    std::cout<<"Created Service with\nService-ID:\t" <<static_cast<char>(*message_service.get_ServiceID())<<"\nInstance-ID:\t"<<static_cast<char>(*message_service.get_InstanceID())<<std::endl;

    

    // Impart PDU to Service till rec-stack is full
    String sample_pdu = "\x01\x0F:" "Hello";
    while (message_service.impart_pdu(&sample_pdu))
    {
        std::cout<<"Added new PDU to Service Rec-Stack"<<std::endl;
    }
    // Process the incoming messages 
    message_service.stackProcessing();

    // Print the Payload provided by the service (default: same as incoming payload)
    while (message_service.responseAvailable())
    {
        std::cout<<"Response PDU:\t"<<message_service.get_response()<<std::endl;
        message_service.clearResponse();
    }
}
