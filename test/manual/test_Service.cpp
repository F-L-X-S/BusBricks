#include "Service.h"
#include "Message.h"

#define STACKSIZE 3
#define SERVICEID 0x6D // ASCII: "m"

int main() {
    // Instantiate  simple Service for "Message"-content  
    Service<Message, STACKSIZE> message_service(SERVICEID);
    std::cout<<"Created Service with\nService-ID:\t" <<static_cast<char>(*message_service.get_ServiceID())<<std::endl;

    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    std::string sample_pdu = "\x01\x0F:" "Hello";

    // Impart PDU to Service till rec-stack is full
    bool recStack_not_full = true;
    while (recStack_not_full)
    {
        recStack_not_full = message_service.impart_pdu(&sample_pdu);
        if (recStack_not_full)
        {
            std::cout<<"Added new PDU to Service Rec-Stack"<<std::endl;
        }
        std::cout<<"Response PDU:\t"<<message_service.get_response()<<std::endl;
    }
    std::cout<<"Rec-Stack full"<<std::endl;
}