/**
 * @file Message_service.cpp
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


#include "Message_service.h"

// Constructor for creating Message-service with deafult Service-ID "m"
Message_service::Message_service(uint8_t instance_id): Service<Message, STACKSIZE>(FUNCTIONCODE, instance_id){}

// Constructor for creating Message-service with custom Service-ID 
Message_service::Message_service(uint8_t instance_id, uint8_t service_id): Service<Message, STACKSIZE>(service_id, instance_id){}

// process all messages from the receive-stack, till the stack is empty
// add all necessary messages to the send-stack 
void Message_service::stackProcessing() {
    // abort, if no new messages available 
    if (rec_stack.empty()) return;   

    // process the rec-stack till it's empty                                                   
    while(!rec_stack.empty()){
        // get last element from receive stack
        Message* last_msg = rec_stack.getElement();
        
        // Print received Message to terminal
        printMessage(last_msg);

        // Ack the received Message
        sendAck(last_msg);

        // delete the element the response was generated for                                           
        rec_stack.deleteElement(0);       
    };                                                            
};

// get the destination Instance-ID for the current response PDU in the send-stack
uint8_t Message_service::get_destinationId(){
    return send_stack.getElement()->get_content()->receiver_id; 
};

// Add the Ack for the message to the send-stack
void Message_service::sendAck(Message* message){
    // extract the content of the message 
    Message_content_t* content = message->get_content();
    // generate the ACK-Response 
    Message_content_t response_content;
    response_content.sender_id = instanceID;                                           // own instance-id as sender                     
    if (static_cast<String>(content->msg_text)!="\0"){  
        response_content.receiver_id = content->sender_id;                             // receiver is the sender of the received message
        response_content.msg_text = "ACK";                                             // Acknowledgement-text
    }    
    // initialize response-message-Object 
    Message response_msg(&response_content);
    // add the ACK-Message to the Send-Stack
    send_stack.addElement(response_msg);
};

// Send a new Message 
void Message_service::sendMessage(char receiverId, String messagetext){
    Message_content_t response_content;
    response_content.sender_id = instanceID;                                           // own instance-id as sender                     
    response_content.receiver_id = receiverId;                           
    response_content.msg_text = messagetext;                                           

    // initialize response-message-Object 
    Message response_msg(&response_content);
    // add the ACK-Message to the Send-Stack
    send_stack.addElement(response_msg);
};

// Print the Received Messages
void Message_service::printMessage(Message* message){
    Serial.println(message->to_string());
};