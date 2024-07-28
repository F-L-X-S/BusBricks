#include "Frame.h"

// Default Constructor
Frame::Frame() : Content<pduString, frameString>(frameString()) {}

// Constructor for creating Frame from PDU
Frame::Frame(pduString* pdu) : Content<pduString, frameString>(*pdu) {}

// Constructor for creating Frame-Object from byte-frame 
Frame::Frame(frameString* frame) : Content<pduString, frameString>(*frame) {}

// Destructor
Frame::~Frame() {}

// Get string-formatted PDU (Content) from Frame-Object
String Frame::getPDU(){
    pduString p = *get_content();
    return String(p);
}

// Get string formatted byte-Frame (Representation) from Frame-Object 
String Frame::getFrame(){
    frameString* str = get_representation();
    return String(*str);
}