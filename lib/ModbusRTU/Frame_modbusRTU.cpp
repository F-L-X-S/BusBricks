#include "Frame_modbusRTU.h"

// Construct Modbus-RRU-Frame from given PDU (Content)
Frame_modbusRTU::Frame_modbusRTU(pduString* pdu, char* slaveId, char* functionCode) : 
    Frame(pdu), slaveId(*slaveId), functionCode(*functionCode) {
    content_to_rep();
};

// Construct Modbus-RRU-Frame from given Byte-Frame (Representation)
Frame_modbusRTU::Frame_modbusRTU(frameString* frame) : 
    Frame(frame) {
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
void Frame_modbusRTU::content_to_rep(){
        size_t pduLength = content.length();                                // get the length of the Content (PDU)
        size_t buffersize = PREFIXSIZE+SUFFIXSIZE+pduLength + 1;            // Calculate the necessary size for the buffer + null-termination
        char* buffer = new char[buffersize];                                // Create representation-buffer 

        // Prefix
        buffer[0] = slaveId;                                               // Byte 0:  Slave-Address
        buffer[1] = functionCode;                                          // Byte 1:  Modbus-RTU function-code 
        
        // Content
        for (size_t i = 0; i < pduLength; ++i) {                       
            buffer[i+PREFIXSIZE] = content[i];                              // Byte 2 to end of PDU:  Content (PDU)
        }

        // CRC
        uint16_t crc = calcCRC16(buffer, pduLength+PREFIXSIZE);
        buffer[pduLength + PREFIXSIZE] = crc & 0xFF;                        // Last 2 Bytes: Append CRC (Little Endian: Low Byte first, then High Byte)
        buffer[pduLength + PREFIXSIZE + 1] = (crc >> 8) & 0xFF;

        // Null terminator
        buffer[buffersize] = '\0';                                          // Append null-termination to the buffer
        representation = buffer;                                            // Set representation to point to the buffer
};

// Convert the given Representation (Frame) to Content (PDU) 
void Frame_modbusRTU::rep_to_content(){
    // PDU (Content)
    size_t len = strlen(representation);                                    // Get the length of the representation (frame) 
    for (size_t i = PREFIXSIZE; i < len-SUFFIXSIZE; ++i) {                  
        content += representation[i];                                       // Write PDU from representation (frame) to Content 
    }

    // Slave-ID 
    slaveId = representation[0];                                           // Set Slave-ID to Byte 0 of the frame 

    // Function-Code
    functionCode = representation[1];                                      // Set the function-Code to Byte 1 of the frame 
};

// Get the Modbus-RTU-function-code of the frame-instance 
char Frame_modbusRTU::getFunctionCode(){
    return functionCode;
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
unsigned short Frame_modbusRTU::calcCRC16(char* buffer, uint8_t size){   
    unsigned short crc16 = CRC16VALUE;
    for (int i = 0; i < size; i++) {
        crc16 ^= static_cast<unsigned char>(buffer[i]);
        for (int j = 0; j < 8; j++) {
            if (crc16 & 0x0001) {
                crc16 = (crc16 >> 1) ^ CRC16MASK;
            } else {
                crc16 >>= 1;
            }
        }
    }
    return crc16;
};

// Check the CRC16-value for the given buffer 
bool Frame_modbusRTU::checkCRC16(){
    uint8_t size = strlen(representation);           // Get the length of the representation 
    char* buffer = new char[size];              
    memcpy(buffer, representation, size);         // copy representation + null-termination to temp-buffer
    bool result = calcCRC16(buffer, size) == 0;     // if rest is 0 crc was correct 
    delete[] buffer;                                
    return result;
};