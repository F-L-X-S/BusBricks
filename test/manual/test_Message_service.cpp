#include "Message_service.h"

#define INSTANCE_ID 0xF

int main(){
    // initialize message-service with instance-id 0xF
    Message_service msg_service(INSTANCE_ID);       
    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    std::string sample_pdu = "\x01\x0F:Hello";

    // Impart PDU to Service till rec-stack is full
    bool recStack_not_full = true;
    while (recStack_not_full)
    {
        recStack_not_full = msg_service.impart_pdu(&sample_pdu);
        if (recStack_not_full)
        {
            std::cout<<"Added new PDU to Service Rec-Stack"<<std::endl;
        }
    }
    std::cout<<"Rec-Stack full"<<std::endl;

    // Ack all Messages in Receive-Stack 
    for (size_t i = 0; i < 5; i++) {
        std::cout << "Response-PDU:\t" << i+1 << ": " << msg_service.get_response() << std::endl;
    }
}