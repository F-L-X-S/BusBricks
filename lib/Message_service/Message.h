#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    #include <cstdio>
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include "Content.h"        // include Content-template

#define MAXPDUSIZE 252

struct Message_content_t{
    char sender_id;                 // Src-ID[1 Byte]
    char receiver_id;               // Rec-ID[1 Byte]
    String msg_text;                // Message-Text

    // Default Constructor
    Message_content_t() : sender_id(0x0), receiver_id(0x0) {
    }

    // Constructor with String parameter
    Message_content_t(char sender_id, char receiver_id, String msg_txt, uint8_t txt_size) : 
        sender_id(sender_id), receiver_id(receiver_id), msg_text(msg_text) {
        msg_text += '\0';
    }
};

// Defines the mapping of string-formatted PDU to a text-message with information for sender and receiver
// Constructor for creating Message from pdu:
//        Message(String* representation)
// Constructor for creating Message from msg-content
//        Message(Message_content_t* message_content)
class Message: public Content<Message_content_t, String>
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
        Message(String* representation);

        // Constructor for creating Message from msg-content
        Message(Message_content_t* message_content);

        //string-representation
        String to_string();
        
    private:
        // Create Message from byte-representation (PDU)
        virtual void rep_to_content() override;

        // Create byte-representation (PDU) from Message
        virtual void content_to_rep() override;



};

#endif // MESSAGE_H