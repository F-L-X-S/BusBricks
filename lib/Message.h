#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
#endif

#include "Content.h"        // include Content-template
#define MAXPDUSIZE 252      // Max size of generated pdu for Modbus-RTU-Frame

struct Message_content_t{
    char msg_text[MAXPDUSIZE-3];    //Rec-ID[1 Byte] Src-ID[1 Byte] 16x3A [1 Byte] Message-Text[249 Byte]
    uint8_t txt_size;               //Size of Message-Text in Bytes
    uint8_t receiver_id;
    uint8_t sender_id;
};

class Message: Content<Message_content_t>
{
    public:
        // Constructor for creating Message from pdu
        Message(unsigned char* pdu, uint8_t* pdu_size);

        // Constructor for creating Message from msg-content
        Message(Message_content_t* message_content);

        // String-representation
        char* to_string();
        
    private:
        // Create Message from frame
        Message_content_t* pdu_to_content(unsigned char* pdu, uint8_t* pdu_size);

        // Create frame from Message-structure
        char* content_to_pdu(Message_content_t* message_content);

};

#endif // MESSAGE_H