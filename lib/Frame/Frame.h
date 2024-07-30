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
// The derived classes define:
//      -the conversion from a given payload plus protocol-specific parameters (e.g. function-code, addresses...)
//      to the frame-representation (override of content_to_rep from Content-Class)
//
//      - the extraction of protocol-specific parameters from the byte-representation (frame) 
//      with eventually checking their validity (e.g. CRC) (override of rep_to_content from Content-Class)
//
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