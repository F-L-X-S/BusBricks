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

    // get the response PDU 
    String get_response() override;
};

#endif // MESSAGE_SERVICE_H