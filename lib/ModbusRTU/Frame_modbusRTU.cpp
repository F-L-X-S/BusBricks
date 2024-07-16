#include "Frame_modbusRTU.h"

// Construct Modbus-RRU-Frame from given PDU (Content)
Frame_modbusRTU::Frame_modbusRTU(pduString* pdu) : Frame(pdu) {
    content_to_rep();
};

// Construct Modbus-RRU-Frame from given Byte-Frame (Representation)
Frame_modbusRTU::Frame_modbusRTU(frameString* frame) : Frame(frame) {
    copy_to_heap(&representation);
    rep_to_content();
};

// Deconstructor 
Frame_modbusRTU::~Frame_modbusRTU(){
    if (representation) {
        delete[] representation;
    }
};

// Convert the given Content (PDU) to Representation (Frame)
void Frame_modbusRTU::content_to_rep(){};

// Convert the given Representation (Frame) to Content (PDU) 
void Frame_modbusRTU::rep_to_content(){
    size_t len = strlen(representation);
    for (size_t i = PREFIXSIZE; i < len-SUFFIXSIZE; ++i) {
        content += representation[i];
    }
};


// Copy the representation pointing to an outside defined char-array to heap-memory
// has to be called to ensure, that destructor is deleting from heap and not from stack
void Frame_modbusRTU::copy_to_heap(const char** str_ptr) {
        size_t len = strlen(*str_ptr);              // Get the length of the referenced string 
        char* buffer = new char[len+1];             // create new char-array on heap 
        for (size_t i = 0; i < len; ++i) {          
            buffer[i] = (*str_ptr)[i];              // copy each character to heap
        }
        buffer[len] = '\0';                         // ensure null-termination
        *str_ptr = buffer;                          // set the reference to the created char-array on heap-memory 
    };

// Calculate the CRC16-value of the given buffer 
short Frame_modbusRTU::calcCRC16(unsigned char* buffer , uint8_t size ){};

// Check the CRC16-value for teh given buffer 
bool Frame_modbusRTU::checkCRC16(unsigned char* buffer ,short CRC){};