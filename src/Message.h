#ifndef MESSAGE_H
#define MESSAGE_H

#include "Content.h"
    
#define MAXPDUSIZE 252

struct Message_content_t{
    char msg_text[MAXPDUSIZE-2];
    char receiver_id;
    char sender_id;
};

class Message: Content<Message_content_t>
{
    private:
        // Create Message from frame
        Message_content_t* pdu_to_content(unsigned char* frame) {
            Message_content_t* msg_content = new Message_content_t(); // store new message-content and safe pointer
            //
            //
            //
            return msg_content;
        }

        // Create frame from Message-structure
        char* content_to_pdu() {
            char* pdu = new char[MAXPDUSIZE]; // store new pdu and safe pointer
            //
            //
            //
            return pdu; // return pointer to pdu
        }

    public:
        // Constructor for creating Message from frame
        Message(unsigned char* frame) : Content(pdu_to_content(frame)) {}

        // Constructor for creating Message from msg-content
        Message(Message_content_t* message_content) : Content(message_content) {}

};

#endif