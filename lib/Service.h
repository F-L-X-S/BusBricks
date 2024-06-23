#ifndef SERVICE_H
#define SERVICE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
#endif

#include "Content.h"
#include "Content_stack.h"

// Template to create a Service class by defining the Content (derived Class of "Content") to handle and the size of the service-stacks
template<typename content_class, int stackSize>                                       
class Service{
private: 
    uint8_t function_code;                                                                                      // Modbus-rtu function code                                  
    Content_stack<content_class, stackSize> rec_stack;                                                          // stack for received content-elements
    unsigned char response_pdu[MAXPDUSIZE];                                                                     // PDU with response 

public: 
    Service(uint8_t function_code): function_code(function_code){}     

    bool impart_pdu(unsigned char* pdu, uint8_t* pdu_size){                                                     // impart new PDU to Service  
        content_class content(pdu, pdu_size);                                                                   // create Content-object from PDU
        return rec_stack.addElement(content);                                                                   // add Content-Object to Rec-Stack
    };

    unsigned char* get_response(){                                                                              // get the response-pdu from the service 
        strcpy((char*)response_pdu, "ACK");
        return response_pdu;
    };                                                                      
                               
};

#endif // SERVICE_H