#include "Message.h"

// Default Constructor
Message::Message() : Content<Message_content_t>(new Message_content_t()) {}

// Destructor
Message::~Message() {
    if (string_rep!=nullptr)
    {
        delete[] string_rep;
    }
}

// Constructor for creating Message from boolean expression
// Message is constructed using the default constructor of Message_Content_t:
// Sender: 0x0; Receiver: 0x0; Text: \0
Message::Message(bool boolean_expression) : Content<Message_content_t>(new Message_content_t()) {}

// Constructor for creating Message from PDU
Message::Message(char* byte_rep, uint8_t* rep_size) : Content(){}

// Constructor for creating Message from msg-content
Message::Message(Message_content_t* message_content) : Content(message_content){}

// Create Message from byte-formatted PDU 
Message_content_t* Message::rep_to_content(char* byte_rep, uint8_t* rep_size) {
    Message_content_t* message_content = new Message_content_t();               // store new message-content and safe pointer
    message_content->sender_id = static_cast<uint8_t>(pdu[0]);                  // get sender-ID from PDU 
    message_content->receiver_id = static_cast<uint8_t>(pdu[1]);                // get receiver-ID from PDU
    message_content->txt_size = *pdu_size-3;
    
    for (int i = 0; i < message_content->txt_size; ++i) {
        message_content->msg_text[i] = static_cast<char>(pdu[3+i]);             // copy message from pdu
    };

    return message_content;                                                     // return message_content-pointer
};

// Create PDU from Message-Object, return ptr to pdu
char* Message::content_to_rep() {
    unsigned int txt_size = static_cast<uint8_t>(content->txt_size);            // get message-size
    uint8_t pdu_size = txt_size+4;
    pdu = new char[pdu_size];                                                   // allocate storage for pdu-array 
    memset(pdu, '\0', pdu_size);                                                // initialize pdu-array with zeros
    pdu[0]=static_cast<char>(content->sender_id);                               // add receiver-ID to PDU 
    pdu[1]=static_cast<char>(content->receiver_id);                             // add sender-ID to PDU 
    pdu[2]=static_cast<char>(0x3A);                                             // add delimeter to PDU 
    for (int i = 0; i < txt_size; ++i) {
        pdu[3+i] = content->msg_text[i];                                        // copy message to pdu
    };
    return pdu;                                                                 // return pdu-ptr
};

// String Represenation of the Message-Object 
char* Message::to_string(){
    string_rep = new char[MAXPDUSIZE];
    char temp[3];

    //append sender-ID
    strcpy(string_rep, "Sender: ");
    snprintf(temp, sizeof(temp), "%02X", content->sender_id);
    strcat(string_rep, temp);

    // append receiver-ID
    strcat(string_rep, "\t\tReceiver: ");
    snprintf(temp, sizeof(temp), "%02X", content->receiver_id);
    strcat(string_rep, temp);

    //append message-text
    strcat(string_rep, "\n");
    strcat(string_rep, content->msg_text);
    strcat(string_rep, "\n");

    return string_rep;
};

