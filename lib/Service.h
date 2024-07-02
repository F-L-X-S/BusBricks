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
// A Service has to be instanciated with a unique modbus-rtu function code and an instance-id, the service-instance should use for it's communication
template<typename content_class, int stackSize>                                       
class Service{
protected: 
    uint8_t function_code;                                                                             // Modbus-rtu function code  
    uint8_t instance_id;                                                                               // service-instance-id                                
    Content_stack<content_class, stackSize> rec_stack;                                                 // stack for received content-elements
    char response_pdu[MAXPDUSIZE];                                                                     // PDU with response 
    
    // write the response-PDU to the reserved memory of the service-instance 
    void write_response_pdu(content_class response_object){
        response_object.get_pdu(&(response_pdu[0]));                                                   // write PDU of last element in Rec-Stack to Response-PDU 
    }

public: 
    Service(uint8_t function_code, uint8_t instance_id): function_code(function_code), instance_id(instance_id){}  

    // Add a new Content-Object created from PDU to the services receive-Stack
    bool impart_pdu(char* pdu, uint8_t* pdu_size){                                                     
        content_class content(pdu, pdu_size);                                                          // create Content-object from PDU
        return rec_stack.addElement(content);                                                          // add Content-Object to Rec-Stack
    };

    // get the response-pdu stored at the reserved memory of the service instance  
    virtual char* get_response(){                                                                      
        // generic response element 
        content_class response_element;
        // write pdu from this element to response-pdu-memory of service-instance 
        write_response_pdu(response_element);
        // return the response-pdu of the service-instance
        return response_pdu;
    };                                                                      
                               
};

#endif // SERVICE_H