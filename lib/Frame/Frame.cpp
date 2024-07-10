#include "Frame.h"

// Default Constructor
Frame::Frame() : Content<pduString, frameString>(pduString()) {
    Frame::content_to_rep();
}

// Constructor for creating Frame-Object from PDU
Frame::Frame(pduString* pdu) : Content(*pdu){
    Frame::content_to_rep();
}

// Constructor for creating Frame-Object from byte-Frame
Frame::Frame(frameString* frame) : Content(*frame){
    Frame::rep_to_content();
}

// Destructor
Frame::~Frame() {}

// Get PDU from Frame-Object as wrapper-function
pduString Frame::getPDU(){
    pduString* pdu = get_content();
    return *pdu;
}

// Get byte-Frame from Frame-Object as wrapper-function
frameString Frame::getFrame(){
    frameString* frame = get_representation();
    return *frame;
}

// Call Frame-Wrapper-function
void Frame::rep_to_content(){
    frame_to_pdu();
}

// Call Frame-Wrapper-function 
void Frame::content_to_rep(){
    pdu_to_frame();
}