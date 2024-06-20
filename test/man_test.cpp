#include "Message.h"

int main() {
    //Create a sample message-content with Sender 0x1 and Receiver 0xF
    Message_content_t msg_content;
    msg_content.sender_id = 0x1;
    msg_content.receiver_id = 0xF;
    strcpy(msg_content.msg_text, "Hello");   
    Message msg_from_content(&msg_content);
    cout<<msg_from_content.to_string()<<std::endl;

    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    unsigned char sample_pdu[] = {0x1, 0xF, 'H', 'e', 'l', 'l', 'o'};
    uint8_t pdu_size = 7; // Size of PDU
    Message msg_from_pdu(sample_pdu, &pdu_size);
    cout<<msg_from_pdu.to_string()<<std::endl;
    return 0;
}
