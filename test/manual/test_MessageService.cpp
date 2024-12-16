/**
 * @file test_MessageService.cpp
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


#include "MessageService.h"

#define INSTANCE_ID 'A'

int main(){
    // initialize message-service with instance-id 0xF
    char instanceId = INSTANCE_ID;
    MessageService msg_service(instanceId);       
    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    std::string sample_pdu = "BA:Hello";

    // Impart PDU to Service till rec-stack is full
    bool recStack_not_full = true;
    while (recStack_not_full)
    {
        recStack_not_full = msg_service.impart_pdu(&sample_pdu);
        if (recStack_not_full)
        {
            std::cout<<"Added new PDU to Service Rec-Stack"<<std::endl;
        }
    }
    std::cout<<"Rec-Stack full"<<std::endl;
    msg_service.stackProcessing();
    // Ack all Messages in Receive-Stack 
    for (size_t i = 0; i < 6; i++) {
        std::cout << "Response-PDU:\t" << i+1 << ": " << msg_service.get_response() << std::endl;
        msg_service.clearResponse();
    }
}