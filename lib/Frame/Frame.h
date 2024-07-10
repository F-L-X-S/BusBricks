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

// Typedefinitions for Constructor-parameters
using pduString = std::string;
using frameString = std::string;


// Frame-Class as derived class from Content
// In the abstract concept of "Content" the byte-Frame itself is interpreted as the Content-Representation
// The "Content"-Object represents the PDU transmitted to the services 
class Frame: protected Content<pduString, frameString>{
    public:
        // Default Constructor 
        Frame();

        // Destrucor
        ~Frame();

        // Constructor for creating Frame from pdu
        Frame(pduString* pdu);

        // Constructor for creating Frame-Object from byte-frame 
        Frame(frameString* frame);

        // get the pdu from the frame-object
        pduString getPDU();

        // get the byte-frame from the frame-object  
        frameString getFrame();

    protected:
        std::string* pdu;
        std::string* frame;

        // Convert PDU to Frame
        // Create Frame (byte-representation) from PDU (Content)
        virtual void pdu_to_frame()=0;

        // Convert Frame to PDU
        // Create PDU (Content) from Frame (byte-representation)
        virtual void frame_to_pdu()=0;

    private:
        // Create PDU (Content) from Frame (byte-representation)
        virtual void rep_to_content() override;

        // Create Frame (byte-representation) from PDU (Content)
        virtual void content_to_rep() override;
};
#endif // FRAME_H