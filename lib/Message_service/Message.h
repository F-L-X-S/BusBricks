#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    #include <cstdio>
    using namespace std;
#endif

#include "Content.h"        // include Content-template

#define MAXPDUSIZE 252

struct Message_content_t{
    char sender_id;              // Src-ID[1 Byte]
    char receiver_id;            // Rec-ID[1 Byte]
    char msg_text[MAXPDUSIZE-3];    // Message-Text[249 Byte]
    uint8_t txt_size;               // Size of Message-Text in Bytes

    // Default Constructor
    Message_content_t() : sender_id(0x0), receiver_id(0x0), txt_size(1) {
        memset(msg_text, 0, sizeof(msg_text));
    }

    // Constructor with std::string parameter
    Message_content_t(char sender_id, char receiver_id, const std::string& msg_txt, uint8_t txt_size) : 
        sender_id(sender_id), receiver_id(receiver_id), txt_size(txt_size) {
        // Kopieren der Zeichenkette und Sicherstellen der Null-Terminierung
        strncpy(msg_text, msg_txt.c_str(), sizeof(msg_text) - 1);
        msg_text[sizeof(msg_text) - 1] = '\0';
    }
};

// Defines the mapping of string-formatted PDU to a text-message with information for sender and receiver
class Message: public Content<Message_content_t, std::string>
{
    public:
        // Default Constructor 
        Message();

        // Destrucor
        ~Message();

        // Constructor for creating Message from boolean expression
        // Message is constructed using the default constructor of Message_Content_t:
        // Sender: 0x0; Receiver: 0x0; Text: \0
        Message(bool boolean_expression);

        // Constructor for creating Message from pdu
        Message(std::string* representation);

        // Constructor for creating Message from msg-content
        Message(Message_content_t* message_content);

        //string-representation
        std::string to_string();
        
    private:
        // Create Message from byte-representation (PDU)
        virtual void rep_to_content() override;

        // Create byte-representation (PDU) from Message
        virtual void content_to_rep() override;



};

#endif // MESSAGE_H