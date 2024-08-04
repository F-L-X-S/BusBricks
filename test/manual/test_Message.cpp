#include "Message.h"

int main() {
    //Create a sample message-content with Sender 0x1 and Receiver 0xF
    Message_content_t msg_content;
    msg_content.sender_id = 0x1;
    msg_content.receiver_id = 0xF;
    msg_content.msg_text = "Hello";   
    Message msg_from_content(&msg_content);
    std::cout<<"\nMessage from Content:\n"<<msg_from_content.to_string()<<std::endl;
    std::cout<<"\nPDU:\n"<<std::endl;
    std::cout<<*msg_from_content.get_representation()<<std::endl;

    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    std::string sample_pdu = "\x01\x0F:" "Hello";
    Message msg_from_pdu(&sample_pdu);
    std::cout<<"\nMessage from PDU:\n"<<msg_from_pdu.to_string().c_str()<<std::endl;
    std::cout<<"\nPDU:\n"<<std::endl;
    std::cout<<*msg_from_pdu.get_representation()<<std::endl;

    // Test destructor 
    Message* msg_ptr= new Message();
    std::cout<<"\nContent Message from Default-Constructor:\n"<<msg_ptr->to_string()<<std::endl;
    std::cout<<"\nPDU:\n"<<std::endl;
    std::cout<<*msg_ptr->get_representation()<<std::endl;
    delete msg_ptr;
    std::cout<<"\nDeleted Message-Instance\n"<<std::endl;

    return 0;
}
