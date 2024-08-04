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
        String message_text = "ACK";                                                   // Acknowledgement 
        strncpy(response_content.msg_text, message_text.c_str(), sizeof(message_text));     
        response_content.txt_size = 3;
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
    String message_text = messagetext;                                           
    strncpy(response_content.msg_text, message_text.c_str(), sizeof(message_text));     
    response_content.txt_size = messagetext.length();

    // initialize response-message-Object 
    Message response_msg(&response_content);
    // add the ACK-Message to the Send-Stack
    send_stack.addElement(response_msg);
};