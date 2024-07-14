#include "Message.h"

// Default Constructor
Message::Message() : Content<Message_content_t, std::string>(Message_content_t()) {
    Message::content_to_rep();
}

// Destructor
Message::~Message() {}

// Constructor for creating Message from boolean expression
// Message is constructed using the default constructor of Message_Content_t:
// Sender: 0x0; Receiver: 0x0; Text: \0
Message::Message(bool boolean_expression) : Content<Message_content_t, std::string>(Message_content_t()) {
    Message::content_to_rep();
}

// Constructor for creating Message from PDU
Message::Message(std::string* representation) : Content<Message_content_t, std::string>(*representation){
    Message::rep_to_content();
}

// Constructor for creating Message from msg-contents
Message::Message(Message_content_t* message_content) : Content(*message_content){
    Message::content_to_rep();
}

// Allocate  Message-content from byte-formatted representation (PDU)
void Message::rep_to_content() {    
    content.sender_id = static_cast<char>(representation[0]);                                      // get sender-ID from PDU 
    content.receiver_id = static_cast<char>(representation[1]);                                    // get receiver-ID from PDU
    content.txt_size = static_cast<uint8_t>(representation.length()) - 3;                               // calculate txt_size

    for (size_t i = 0; i < content.txt_size; ++i) {
        content.msg_text[i] = representation[3 + i];                                                  // copy message from PDU
    }                                                                                                        // return message_content-pointer
};

// Allocate byte-values for representation from Message-Object
void Message::content_to_rep() {
    representation = "";
    representation += static_cast<char>(content.sender_id);                                                 // Add Sender-ID
    representation += static_cast<char>(content.receiver_id);                                               // Add Receiver-ID
    representation += static_cast<char>(0x3A);                                                              // Add ASCII ":"
    representation += std::string(content.msg_text);                                                        // Append message text 
};

// String Represenation of the Message-Object 
std::string Message::to_string(){
    std::string string_rep;
    char hex_buffer[5]; // Buffer für die Hexadezimaldarstellung

    
    // Sender-ID 
    string_rep = "Sender: ";
    sprintf(hex_buffer, "0x%02X", content.sender_id); 
    string_rep += hex_buffer;

    // Receiver-ID 
    string_rep += "\t\tReceiver: ";
    sprintf(hex_buffer, "0x%02X", content.receiver_id); 
    string_rep += hex_buffer;

    // Message-text
    string_rep += "\n";
    string_rep += content.msg_text;
    string_rep += "\n";

    return string_rep;
};

