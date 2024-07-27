#ifndef FRAME_H
#define FRAME_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include<Content.h>

// Typedefinitions for Constructor-parameters
using pduString = String;
using frameString = const char*;


// Frame-Class as derived class from Content
// In the abstract concept of "Content" the byte-Frame itself is interpreted as the Content-Representation
// The "Content"-Object represents the PDU transmitted to the services 
class Frame: public Content<pduString, frameString>{
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
        String getPDU();

        // get the byte-frame from the frame-object  
        String getFrame();
};
#endif // FRAME_H