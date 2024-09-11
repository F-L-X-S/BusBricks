/**
 * @file Frame_modbusRTU.cpp
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright * Copyright (C) 2024 Felix Schuelke
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#include "Frame_modbusRTU.h"

// Construct Modbus-RRU-Frame from given PDU (Content)
Frame_modbusRTU::Frame_modbusRTU(pduString* pdu, char* slaveId, char* functionCode) : 
    Frame(pdu), slaveId(*slaveId), functionCode(*functionCode) {
    content_to_rep();
};

// Construct Modbus-RTU-Frame from given Byte-Frame (Representation)
Frame_modbusRTU::Frame_modbusRTU(frameString* frame) : 
    Frame(frame) {
    copy_to_heap(&representation);
    rep_to_content();
};

// Construct empty Modbus-RTU-Frame 
Frame_modbusRTU::Frame_modbusRTU() : 
    Frame() {
        content_to_rep();
    };

// Deconstructor 
Frame_modbusRTU::~Frame_modbusRTU(){
    if (buffer!=nullptr) delete[] buffer;
    buffer = nullptr;
};

// Convert the given Content (PDU) to Representation (Frame)
void Frame_modbusRTU::content_to_rep(){
        size_t pduLength = content.length();                                // get the length of the Content (PDU)
        size_t buffersize = PREFIXSIZE+SUFFIXSIZE+pduLength + 1;            // Calculate the necessary size for the buffer + null-termination
        delete[] buffer;
        buffer = new char[buffersize];                                      // Create representation-buffer 

        // Prefix
        buffer[0] = slaveId;                                                // Byte 0:  Slave-Address
        buffer[1] = functionCode;                                           // Byte 1:  Modbus-RTU function-code 
        
        // Content
        for (size_t i = 0; i < pduLength; ++i) {                       
            buffer[i+PREFIXSIZE] = content[i];                              // Byte 2 to end of PDU:  Content (PDU)
        }

        // CRC
        uint16_t crc = calcCRC16(buffer, pduLength+PREFIXSIZE);                     // Last 4 Bytes: Append CRC (Little Endian: Low Byte first, then High Byte)
        sprintf(&buffer[pduLength + PREFIXSIZE], "%02X", (crc & 0xFF));             // Append Hex-formatted Low-Byte as ASCII-chars                
        sprintf(&buffer[pduLength + PREFIXSIZE + 2], "%02X", ((crc >> 8) & 0xFF));  // Append Hex-formatted High-Byte as ASCII-chars    

        // Null terminator
        buffer[buffersize] = '\0';                                          // Append null-termination to the buffer
        representation = buffer;                                            // Set representation to point to the buffer
};

// Convert the given Representation (Frame) to Content (PDU) 
// leaves Content empty, if Frames is shorter than expected
void Frame_modbusRTU::rep_to_content(){
    size_t len = strlen(representation);                                    // Get the length of the representation (frame) 
    if (len > (PREFIXSIZE + SUFFIXSIZE))
    {
        // PDU (Content)
        for (size_t i = PREFIXSIZE; i < len-SUFFIXSIZE; ++i) {                  
            content += representation[i];                                   // Write PDU from representation (frame) to Content 
        }

        // Slave-ID 
        slaveId = representation[0];                                        // Set Slave-ID to Byte 0 of the frame 

        // Function-Code
        functionCode = representation[1];                                   // Set the function-Code to Byte 1 of the frame 
    }
};

// Get the Modbus-RTU-function-code of the frame-instance 
char Frame_modbusRTU::getFunctionCode(){
    return functionCode;
};


// Copy the representation pointing to an outside defined char-array to heap-memory
// has to be called to ensure, that destructor is deleting from heap and not from stack
void Frame_modbusRTU::copy_to_heap(const char** str_ptr) {
        size_t len = strlen(*str_ptr);              // Get the length of the referenced string 
        delete[] buffer;
        buffer = new char[len+1];                   // create new char-array on heap 
        for (size_t i = 0; i < len; ++i) {          
            buffer[i] = (*str_ptr)[i];              // copy each character to heap
        }
        buffer[len] = '\0';                         // ensure null-termination
        *str_ptr = buffer;                          // set the reference to the created char-array on heap-memory 
    };

// Calculate the CRC16-value of the given buffer 
unsigned short Frame_modbusRTU::calcCRC16(char* crcBuffer, uint8_t size){
    Serial.println(size);   
    unsigned short crc16 = CRC16VALUE;
    for (int i = 0; i < size; i++) {
        crc16 ^= static_cast<unsigned char>(crcBuffer[i]);
        for (int j = 0; j < 8; j++) {
            if (crc16 & 0x0001) {
                crc16 = (crc16 >> 1) ^ CRC16MASK;
            } else {
                crc16 >>= 1;
            }
        }
    }
    Serial.println(crc16);
    return crc16;
};

// Check the CRC16-value for the given buffer 
bool Frame_modbusRTU::checkCRC16(){
    uint8_t repSize = strlen(representation);                   // Get the length of the representation 
    char* crcBuffer = new char[repSize-2];                      // create buffer to calc crc for
    memcpy(crcBuffer, representation, repSize-SUFFIXSIZE);      // copy representation without crc 

    // extract CRC as Hex in Ascii-format
    char lowCrcAscii[2], highCrcAscii[2];                              
    memcpy(lowCrcAscii, representation + (repSize-SUFFIXSIZE), 2);
    memcpy(highCrcAscii, representation + (repSize-SUFFIXSIZE+2), 2);

    // convert Ascii-formatted Hex to byte
    char lowCrcByte, highCrcByte;
    sscanf(lowCrcAscii, "%x", &lowCrcByte);
    sscanf(highCrcAscii, "%x", &highCrcByte);

    // append byte-formatted CRC to crc-buffer
    crcBuffer[strlen(crcBuffer) - 1] = lowCrcByte;
    crcBuffer[strlen(crcBuffer)] = highCrcByte;

    bool result = calcCRC16(crcBuffer, strlen(crcBuffer)) == 0;      // if rest is 0 crc was correct 
    delete[] crcBuffer;                                
    return result;
};

// Copy constructor
Frame_modbusRTU::Frame_modbusRTU(const Frame_modbusRTU& other) : Frame(other) {
    // Copy the derived-class-specific attributes
    slaveId = other.slaveId;
    functionCode = other.functionCode;
    Frame::operator=(other);                            // Call base class assignment operator

    // Copy dynamically allocated memory
    if (!buffer) buffer = new char[1];                  // initialize buffer 
    copy_to_heap(&representation);                      // copy buffer from assigned instance and re-allocate representation-ptr to prevent double-free
    
}

// Assignment operator
Frame_modbusRTU& Frame_modbusRTU::operator=(const Frame_modbusRTU& other) {
    if (this == &other) {
        return *this;                                   // handle self-assignment
    }

    // Copy the derived-class-specific attributes
    slaveId = other.slaveId;
    functionCode = other.functionCode;
    Frame::operator=(other);                            // Call base class assignment operator

    // Copy dynamically allocated memory
    if (!buffer) buffer = new char[1];                  // initialize buffer 
    copy_to_heap(&representation);                      // copy buffer from assigned instance and re-allocate representation-ptr to prevent double-free
    
    return *this;
}