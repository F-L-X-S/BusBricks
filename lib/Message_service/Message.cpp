#include "Message.h"

// Default Constructor
Message::Message() : Content<Message_content_t, String>(Message_content_t()) {
    Message::content_to_rep();
}

// Destructor
Message::~Message() {}

// Constructor for creating Message from boolean expression
// Message is constructed using the default constructor of Message_Content_t:
// Sender: 0x0; Receiver: 0x0; Text: \0
Message::Message(bool boolean_expression) : Content<Message_content_t, String>(Message_content_t()) {
    Message::content_to_rep();
}

// Constructor for creating Message from PDU
Message::Message(String* representation) : Content<Message_content_t, String>(*representation){
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
    #ifdef ARDUINO
        content.msg_text = representation.substring(3); 
    #else
        content.msg_text = representation.substr(3); 
    #endif
};

// Allocate byte-values for representation from Message-Object
void Message::content_to_rep() {
    representation = "";
    representation += static_cast<char>(content.sender_id);                                         // Add Sender-ID
    representation += static_cast<char>(content.receiver_id);                                       // Add Receiver-ID
    representation += static_cast<char>(0x3A);                                                      // Add ASCII ":"
    representation += content.msg_text;                                                     // Append message text 
};

// String Represenation of the Message-Object 
String Message::to_string(){
    String string_rep;
    char hex_buffer[5]; // Buffer for hex-representation

    
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
