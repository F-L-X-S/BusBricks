#include "Message.h"

// Constructor for creating Message from frame
Message::Message(unsigned char* pdu, uint8_t* pdu_size) : Content(pdu_to_content(pdu, pdu_size)){}

// Constructor for creating Message from msg-content
Message::Message(Message_content_t* message_content) : Content(message_content){}

// Create Message from pdu 
Message_content_t* Message::pdu_to_content(unsigned char* pdu, uint8_t* pdu_size) {
    Message_content_t* message_content = new Message_content_t();               // store new message-content and safe pointer
    message_content->receiver_id = static_cast<uint8_t>(pdu[0]);                // get receiver-ID from PDU 
    message_content->sender_id = static_cast<uint8_t>(pdu[1]);                  // get sender-ID from PDU
    message_content->txt_size = static_cast<uint8_t>(pdu[2]);                  // get sender-ID from PDU
    
    for (int i = 0; i < *pdu_size; ++i) {
        message_content->msg_text[i] = static_cast<char>(pdu[i+2]);             // copy message from pdu
    };

    return message_content;                                                     // return message_content-pointer
}

// Create frame from Message-structure
char* Message::content_to_pdu(Message_content_t* message_content) {
    char* pdu = new char[MAXPDUSIZE];                                           // store new pdu and safe pointer
    pdu[0]=static_cast<uint8_t>(message_content->receiver_id);                  // add receiver-ID to PDU 
    pdu[1]=static_cast<uint8_t>(message_content->sender_id);                    // add sender-ID to PDU 

    unsigned int txt_size = static_cast<uint8_t>(message_content->txt_size);            // get message-size
    for (int i = 0; i < txt_size; ++i) {
        pdu[3+i] = static_cast<char>(message_content->msg_text[i]);             // copy message to pdu
    };

    return pdu;                                                                 // return pdu-pointer
};

// String Represenation 
char* Message::to_string(){
    char* str = new char[MAXPDUSIZE];
    // append receiver-ID
    strcpy(str, "Receiver:\t");
    strncat(str, &content->receiver_id, 1);
    //append sender-ID
    strcat(str, "Sender:\t");
    strncat(str, &content->sender_id, 1);
    //append text
    strcat(str, "\n");
    strcat(str, content->msg_text);
    return str;
};

