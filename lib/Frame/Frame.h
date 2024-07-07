#ifndef FRAME_H
#define FRAME_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    using namespace std;
#endif
#include<Content.h>

// Frame-Class as derived class from Content
// In the abstract concept of "Content" the byte-Frame itself is interpreted as the Content-PDU
// The "Content"-Object represents the PDU transmitted to the services 
class Frame: protected Content<char>{
    public:
        // Default Constructor 
        Frame();

        // Destrucor
        ~Frame();

        // Constructor for creating Frame from pdu
        Frame(char* pdu);

        // Constructor for creating Frame-Object from byte-frame 
        Frame(char* frame);

        // get the pdu from the frame-object
        char* getPDU();

        // get the byte-frame from the frame-object  
        char* getFrame();

    private:
        virtual char* pdu_to_frame(char* pdu)=0;
        virtual char* frame_to_pdu(char* frame)=0;
};
#endif // FRAME_H