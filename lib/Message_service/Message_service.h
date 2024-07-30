#ifndef MESSAGE_SERVICE_H
#define MESSAGE_SERVICE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    #include <cstring>
    using namespace arduinoMocking;    
#endif

#include "Service.h"
#include "Message.h"

#define STACKSIZE 3         // Number of elements the services stacks can store 
#define FUNCTIONCODE 0x6D   // ASCII: "m"

// Service-class for handling Content from type "Message"                                  
class Message_service: public Service<Message, STACKSIZE>
{
public:
    // Constructor for creating Message-service 
    Message_service(uint8_t instance_id);

    // process all messages from the receive-stack, till the stack is empty
    // add all necessary messages to the send-stack 
    void stackProcessing() override;

    // get the destination Instance-ID for the current response PDU
    uint8_t get_destinationId();
};

#endif // MESSAGE_SERVICE_H