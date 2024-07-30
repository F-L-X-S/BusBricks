#include "Message_service.h"

// Constructor for creating Message-service 
Message_service::Message_service(uint8_t instance_id): Service<Message, STACKSIZE>(FUNCTIONCODE, instance_id){}

// process all messages from the receive-stack, till the stack is empty
// add all necessary messages to the send-stack 
void Message_service::stackProcessing() {
    if (rec_stack.empty()) return; 
    while(!rec_stack.empty()){
    // get last element from receive stack
    Message last_msg = rec_stack.getElement();
    Message_content_t* last_content = last_msg.get_content();
    // generate the ACK-Response to the last element
    Message_content_t response_content;
    response_content.sender_id = instanceID;
    if (static_cast<String>(last_content->msg_text)!="\0"){  
        response_content.receiver_id = last_content->sender_id;
        String message_text = "ACK";
        strncpy(response_content.msg_text, message_text.c_str(), sizeof(message_text));
        response_content.txt_size = 3;
    }    
    // initialize response-message-Object 
    Message response_msg(&response_content);
    // add the ACK-Message to the Send-Stack
    send_stack.addElement(response_msg);
    // delete the element the response was generated for                                           
    rec_stack.deleteElement(0);       
    };                                                            
};

// get the destination Instance-ID for the current response PDU in the send-stack
uint8_t Message_service::get_destinationId(){
    Message sendMsg = rec_stack.getElement();
    return sendMsg.get_content()->receiver_id; 
};