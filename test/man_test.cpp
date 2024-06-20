#include "Message.h"

int main() {
    Message_content_t msg_content;
    msg_content.receiver_id = 0xB;
    msg_content.sender_id = 0x0A;
    strcpy(msg_content.msg_text, "Hello");   
    // Create a sample PDU with Receiver 0x2 and Sender 0x1
    unsigned char sample_pdu[] = {0x2, 0x1, 'H', 'e', 'l', 'l', 'o'};
    uint8_t pdu_size = 7; // Size of PDU

    Message msg_from_pdu(sample_pdu, &pdu_size);
    Message msg_from_content(&msg_content);

    cout<<msg_from_content.to_string()<<std::endl;
    cout<<msg_from_pdu.to_string()<<std::endl;
    return 0;
}
