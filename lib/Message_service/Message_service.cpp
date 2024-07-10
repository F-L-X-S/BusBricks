#include "Message_service.h"

// Constructor for creating Message-service 
Message_service::Message_service(uint8_t instance_id): Service<Message, STACKSIZE>(FUNCTIONCODE, instance_id){}

// Overwrite default response-PDU by response PDU written by Service-Functions
std::string Message_service::get_response() {
    // get last element from receive stack
    Message last_msg = rec_stack.getElement();
    Message_content_t* last_content = last_msg.get_content();
    // generate the ACK-Response to the last element
    Message_content_t response_content;
    response_content.sender_id = instanceID;
    if (static_cast<std::string>(last_content->msg_text)!="\0"){  
        response_content.receiver_id = last_content->sender_id;
        std::string message_text = "ACK";
        strncpy(response_content.msg_text, message_text.c_str(), sizeof(message_text));
        response_content.txt_size = 3;
    }    

    // initialize response-message-Object 
    Message response_msg(&response_content);
    // write PDU to Response-PDU-memory of the Service-instance
    write_response_pdu(response_msg);
    // delete the element the response was generated for                                           
    rec_stack.deleteElement(0);                                                                   
    return response_pdu;
};