#include <Frame.h>
#include<Message.h>

#define MAXFRAMESIZE 256

class msg_Frame: protected Frame{
    private:
        char* pdu_to_frame(char* pdu){
            
        };
        char* frame_to_pdu(char* frame){

        };
};


int main() {
    //Create a sample message-content with Sender 0x1 and Receiver 0xF
    Message_content_t msg_content;
    msg_content.sender_id = 0x1;
    msg_content.receiver_id = 0xF;
    strcpy(msg_content.msg_text, "Hello");   
    msg_content.txt_size = 5;
    Message msg(&msg_content);

    // Create a generic Frame from the Message-PDU 
    char* msg_pdu = msg.get_pdu();
    Frame msg_frame()
    return 0;
}