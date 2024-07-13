#include "Frame.h"

// Default Constructor
Frame::Frame() : Content<pduString, frameString>(pduString()) {}

// Constructor for creating Frame from PDU
Frame::Frame(pduString* pdu) : Content<pduString, frameString>(*pdu) {}

// Constructor for creating Frame-Object from byte-frame 
Frame::Frame(frameString* frame) : Content<pduString, frameString>(*frame) {}

// Destructor
Frame::~Frame() {}

// Get PDU from Frame-Object as wrapper-function
pduString Frame::getPDU(){
    pduString pdu = *get_content();
    return pdu;
}

// Get byte-Frame from Frame-Object as wrapper-function
frameString Frame::getFrame(){
    frameString frame = *get_representation();
    return frame;
}