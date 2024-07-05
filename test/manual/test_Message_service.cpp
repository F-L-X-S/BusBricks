#include "Message_service.h"

#define INSTANCE_ID 0xF

int main(){
    // initialize message-service with instance-id 0xF
    Message_service msg_service(INSTANCE_ID);       
    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    char sample_pdu[] = {0x1, INSTANCE_ID, ':', 'H', 'e', 'l', 'l', 'o'};
    uint8_t pdu_size = 8; // Size of PDU

    // Impart PDU to Service till rec-stack is full
    bool recStack_not_full = true;
    while (recStack_not_full)
    {
        recStack_not_full = msg_service.impart_pdu(sample_pdu, &pdu_size);
        if (recStack_not_full)
        {
            std::cout<<"Added new PDU to Service Rec-Stack"<<std::endl;
        }
    }
    std::cout<<"Rec-Stack full"<<std::endl;

    // Ack all Messages in Receive-Stack 
    for (size_t i = 0; i < 5; i++) {
        char response_pdu[MAXPDUSIZE];
        strncpy(response_pdu, msg_service.get_response(), sizeof(response_pdu));
        std::cout << "Response-PDU:\t" << i+1 << ": " << response_pdu << std::endl;
    }
}