#include "Message.h"

// Default Constructor
Message::Message() : Content<Message_content_t, std::string>(new Message_content_t()) {}

// Destructor
Message::~Message() {
    if (string_rep!=nullptr){
        delete[] string_rep;
    }
}

// Constructor for creating Message from boolean expression
// Message is constructed using the default constructor of Message_Content_t:
// Sender: 0x0; Receiver: 0x0; Text: \0
Message::Message(bool boolean_expression) : Content<Message_content_t, std::string>(new Message_content_t()) {}

// Constructor for creating Message from PDU
Message::Message(std::string* representation) : Content<Message_content_t, std::string>(representation){}

// Constructor for creating Message from msg-contents
Message::Message(Message_content_t* message_content) : Content(message_content){}

// Allocate  Message-content from byte-formatted representation (PDU)
void Message::rep_to_content() {    
    content.sender_id = static_cast<uint8_t>(representation[0]);                                      // get sender-ID from PDU 
    content.receiver_id = static_cast<uint8_t>(representation[1]);                                    // get receiver-ID from PDU
    content.txt_size = static_cast<uint8_t>(representation.length()) - 3;                               // calculate txt_size

    for (size_t i = 0; i < content.txt_size; ++i) {
        content.msg_text[i] = representation[3 + i];                                                  // copy message from PDU
    }                                                                                                        // return message_content-pointer
};

// Allocate byte-values for representation from Message-Object
void Message::content_to_rep() {
    unsigned int txt_size = static_cast<uint8_t>(content.txt_size);                                     // get message-size
    representation[0]=static_cast<char>(content.sender_id);                                             // Add Sender-ID
    representation[1]=static_cast<char>(content.receiver_id);                                           // Add Receiver-ID
    representation[2]=static_cast<char>(0x3A);                                                               // Add ASCII ":"
    representation += String(content.msg_text);                                                         // Append message text 
};

// String Represenation of the Message-Object 
char* Message::to_string(){
    string_rep = new char[MAXPDUSIZE];
    char temp[3];

    //append sender-ID
    strcpy(string_rep, "Sender: ");
    snprintf(temp, sizeof(temp), "%02X", content.sender_id);
    strcat(string_rep, temp);

    // append receiver-ID
    strcat(string_rep, "\t\tReceiver: ");
    snprintf(temp, sizeof(temp), "%02X", content.receiver_id);
    strcat(string_rep, temp);

    //append message-text
    strcat(string_rep, "\n");
    strcat(string_rep, content.msg_text);
    strcat(string_rep, "\n");

    return string_rep;
};

