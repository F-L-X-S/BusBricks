#include "Message.h"

int main() {
    //Create a sample message-content with Sender 0x1 and Receiver 0xF
    Message_content_t msg_content;
    msg_content.sender_id = 0x1;
    msg_content.receiver_id = 0xF;
    strcpy(msg_content.msg_text, "Hello");   
    msg_content.txt_size = 5;
    Message msg_from_content(&msg_content);
    std::cout<<msg_from_content.to_string()<<std::endl;
    std::cout<<msg_from_content.get_pdu()<<std::endl;

    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    char sample_pdu[] = {0x1, 0xF, ':','H', 'e', 'l', 'l', 'o'};
    uint8_t pdu_size = sizeof(sample_pdu); // Size of PDU
    Message msg_from_pdu(sample_pdu, &pdu_size);
    std::cout<<msg_from_pdu.to_string()<<std::endl;
    std::cout<<msg_from_pdu.get_pdu()<<std::endl;
    return 0;
}
