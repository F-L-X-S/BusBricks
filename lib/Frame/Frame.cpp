#include "Frame.h"

// Default Constructor
// Frame::Frame() : Content<char>(new char) {}

// // Constructor for creating Frame-Object from PDU
// Frame::Frame(char* pdu) : Content(pdu){}

// // Constructor for creating Frame-Object from byte-Frame
// Frame::Frame(char* byteframe) : Content(frame_to_pdu(byteframe)){}

// // Destructor
// Frame::~Frame() {}

// // Get PDU from Frame-Object
// char* Frame::getPDU(){
//     char pdu = get_content();
//     return &pdu;
// }

// // Get byte-Frame from Frame-Object
// char* Frame::getFrame(){
//     char pdu = get_content();
//     return pdu_to_frame(&pdu);
// }