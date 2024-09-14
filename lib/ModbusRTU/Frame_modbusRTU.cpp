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
Frame_modbusRTU::Frame_modbusRTU(String* pdu, char* slaveId, char* functionCode) : 
    Frame(pdu), slaveId(*slaveId), functionCode(*functionCode) {
    content_to_rep();
};

// Construct Modbus-RTU-Frame from given Byte-Frame (Representation)
Frame_modbusRTU::Frame_modbusRTU(CharArray* frame) : 
    Frame(frame) {
    rep_to_content();
};

// Construct empty Modbus-RTU-Frame 
Frame_modbusRTU::Frame_modbusRTU() : 
    Frame() {
        content_to_rep();
    };

// Deconstructor 
Frame_modbusRTU::~Frame_modbusRTU(){
};

// Convert the given Content (PDU) to Representation (Frame)
void Frame_modbusRTU::content_to_rep(){
        size_t pduLength = content.length();                                // get the length of the Content (PDU)
        
        // Prefix
        representation[0] = slaveId;                                                // Byte 0:  Slave-Address
        representation[1] = functionCode;                                           // Byte 1:  Modbus-RTU function-code 
        
        // Content
        for (size_t i = 0; i < pduLength; ++i) {                       
            representation[i+PREFIXSIZE] = content[i];                              // Byte 2 to end of PDU:  Content (PDU)
        }

        // CRC
        uint16_t crc = calcCRC16(representation.getData(), pduLength+PREFIXSIZE);
        representation[pduLength + PREFIXSIZE] = crc & 0xFF;                        // Last 2 Bytes: Append CRC (Little Endian: Low Byte first, then High Byte)
        representation[pduLength + PREFIXSIZE + 1] = (crc >> 8) & 0xFF;

        // Null terminator
        representation += '\0';                                          // Append null-termination to the buffer
};

// Convert the given Representation (Frame) to Content (PDU) 
// leaves Content empty, if Frames is shorter than expected
void Frame_modbusRTU::rep_to_content(){
    // Framesize is has payload
    if (representation.getSize() > (PREFIXSIZE + SUFFIXSIZE))
    {
        // PDU (Content)
        for (size_t i = PREFIXSIZE; i < representation.getSize()-SUFFIXSIZE; ++i) {                  
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

// Calculate the CRC16-value of the given buffer 
unsigned short Frame_modbusRTU::calcCRC16(char* crcBuffer, uint8_t size){   
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
    return crc16;
};

// Check the CRC16-value of the frame  
bool Frame_modbusRTU::checkCRC16(){
    // if rest is 0 crc was correct   
    return calcCRC16(representation.getData(), representation.getSize()) == 0;                              
};
