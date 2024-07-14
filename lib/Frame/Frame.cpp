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
std::string Frame::getPDU(){
    pduString p = *get_content();
    return std::string(p);
}

// Get byte-Frame from Frame-Object as wrapper-function
std::string Frame::getFrame(){
    frameString* str = get_representation();
    return std::string(*str);
}