#include "Service.h"
#include "Message.h"

#define STACKSIZE 3
#define SERVICEID 0x6D // ASCII: "m"
#define INSTANCE_ID 0xF

int main() {
    // Instantiate  simple Service for "Message"-content  
    Service<Message, STACKSIZE> message_service(SERVICEID, INSTANCE_ID);
    std::cout<<"Created Service with\nService-ID:\t" <<static_cast<char>(*message_service.get_ServiceID())<<"\nInstance-ID:\t"<<static_cast<char>(*message_service.get_InstanceID())<<std::endl;

    

    // Impart PDU to Service till rec-stack is full
    String sample_pdu = "\x01\x0F:" "Hello";
    while (message_service.impart_pdu(&sample_pdu))
    {
        std::cout<<"Added new PDU to Service Rec-Stack"<<std::endl;
    }
    // Process the incoming messages 
    message_service.stackProcessing();

    // Print the Payload provided by the service (default: same as incoming payload)
    while (message_service.responseAvailable())
    {
        std::cout<<"Response PDU:\t"<<message_service.get_response()<<std::endl;
        message_service.clearResponse();
    }
}
