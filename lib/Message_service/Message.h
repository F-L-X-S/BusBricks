#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
#endif

#include "Content.h"        // include Content-template

struct Message_content_t{
    uint8_t sender_id;              // Src-ID[1 Byte]
    uint8_t receiver_id;            // Rec-ID[1 Byte]
    char msg_text[MAXPDUSIZE-3];    // Message-Text[249 Byte]
    uint8_t txt_size;               // Size of Message-Text in Bytes

    // Default Constructor
    Message_content_t() : sender_id(0x0), receiver_id(0x0), txt_size(1) {
        memset(msg_text, 0, sizeof(msg_text));
    }

    // Constructor with std::string parameter
    Message_content_t(uint8_t sender_id, uint8_t receiver_id, const std::string& msg_txt, uint8_t txt_size) : 
        sender_id(sender_id), receiver_id(receiver_id), txt_size(txt_size) {
        // Kopieren der Zeichenkette und Sicherstellen der Null-Terminierung
        strncpy(msg_text, msg_txt.c_str(), sizeof(msg_text) - 1);
        msg_text[sizeof(msg_text) - 1] = '\0';
    }
};

// Defines the mapping of Byte formatted PDU to a text-message with information for sender and receiver
class Message: public Content<Message_content_t>
{
    public:
        // Default Constructor 
        Message();

        // Constructor for creating Message from boolean expression
        // Message is constructed using the default constructor of Message_Content_t:
        // Sender: 0x0; Receiver: 0x0; Text: \0
        Message(bool boolean_expression);

        // Constructor for creating Message from pdu
        Message(char* pdu, uint8_t* pdu_size);

        // Constructor for creating Message from msg-content
        Message(Message_content_t* message_content);

        // Create frame from Message-structure
        bool get_pdu(char* pdu_destination);

        // String-representation
        char* to_string();
        
    private:
        // Create Message from frame
        Message_content_t* pdu_to_content(char* pdu, uint8_t* pdu_size);

};

#endif // MESSAGE_H