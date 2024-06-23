#ifndef CONTENT_H
#define CONTENT_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
#endif

#define MAXPDUSIZE 252      // Max size of generated pdu for Modbus-RTU-Frame

template<typename content_type>
class Content{
public: 
    Content(content_type* data = nullptr) : content(data) {}                                // Construct Content-class with an unspecific type

protected:
    content_type* content;                                                                  // Pointer to unspecified type, depends on type of payload 
    virtual char* content_to_pdu(content_type* content)=0;                                  // has to be defined in derived class (=0)
    virtual content_type* pdu_to_content(unsigned char* pdu, uint8_t* pdu_size)=0;          // has to be defined in derived class (=0)                                                       
};

#endif // CONTENT_H