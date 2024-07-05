#ifndef CONTENT_H
#define CONTENT_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
#endif

#define MAXPDUSIZE 252      // Max size of generated pdu for Modbus-RTU-Frame

// Template for defining a Class for mapping byte-formatted PDU to a processable data-structure
template<typename content_type>
class Content{
public: 
    // Construct Content-class with an unspecific type
    Content(content_type* data = nullptr) : content(data) {}                                

    // Destroy Content-element
    // delete all dynamically allocated memories
    virtual ~Content() {
        if (pdu!=nullptr)
        {
            delete[] pdu;
        }
    }
    
    // write the pdu to a destination char-array, has to be defined in derived class (=0)
    virtual char* get_pdu()=0;                               

    // Get the Content formatted as content_type from the Object 
    content_type get_content(){                                                             
        return *content;
    };

protected:
    content_type* content;                                                // Pointer to unspecified type, depends on type of payload 
    char* pdu=nullptr;                                                    // PDU, dynamically allocated in get_pdu 

    // Convert the byte-formatted PDU to a content-object, has to be defined in derived class (=0)
    virtual content_type* pdu_to_content(char* pdu, uint8_t* pdu_size)=0;                                                                        
};

#endif // CONTENT_H