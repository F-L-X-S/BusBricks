#ifndef MESSAGE_SERVICE_H
#define MESSAGE_SERVICE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
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
    std::string get_response() override;
};

#endif // MESSAGE_SERVICE_H