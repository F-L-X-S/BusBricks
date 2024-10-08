/**
 * @file CommInterface_modbusRTU.cpp
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

#include "CommInterface_modbusRTU.h"

// Construct Modbus-RTU-Communication-Interface predefined SoftwareSerial-Inetrface
CommInterface_modbusRTU::CommInterface_modbusRTU(SoftwareSerial* softwareserial, uint16_t baudrate, char deviceId) : 
    deviceId(deviceId),
    CommInterface<SoftwareSerial>(softwareserial, baudrate) {
        _calculateTimeouts(baudrate);
};

// Destructor
CommInterface_modbusRTU::~CommInterface_modbusRTU() {
}

// Sending
bool CommInterface_modbusRTU::send(){    
    if (sendBuffer==nullptr) return false;                            // check, if the sendbuffer-ptr is set to a valid memory
    interface->write(sendBuffer->getData(), sendBuffer->getSize());   // write the byte-converted string to the interface  
    interface->flush();                                               // ensure, interfaces sending-buffer is completely empty before returning
    return true;
};

// Receiving 
// the received Frame is directly written to the specified receive-buffer
// after a frame was received within the receive-timeout, the function returns true
// the Comm-Interfece is not checking any Content of the frame 
bool CommInterface_modbusRTU::receive(){
    unsigned long startTime = micros();                             // Time, the function gets called
    uint16_t numBytes = 0;                                          // Received number of bytes
    bool receivingFlag = (deviceId == '\0') ? true:false;           // interface started receiving a frame (Slavemode: only if adressed to the device-ID, Mastermode: each frame) 

    // Wait for a relevant Frame 
    while (!interface->available()) {
    if (micros() - startTime >= _recTimeout) {
      return false;                                                 // No Frame received in specified timespan
    }
  }

  startTime = micros();                                             // redefine the time for measuring timeouts

  // Receive a relevant frame as long as timeout and frame length are ok
  bool appendNullByte = false;
  while (numBytes < MAXFRAMESIZE) {
        // check if the char in buffer is the device-ID or receiving started already
        if (interface->available() && (receivingFlag || (deviceId==interface->peek())))     
        {
            receivingFlag = true;                                                       // Set the receive-flag 
            startTime = micros();                                                       // redefine the time for measuring timeouts

            // Append a nullbyte
            if (appendNullByte){
              *receiveBuffer+= '\0';                          
              appendNullByte = false;}

            *receiveBuffer+= char(interface->read());                                   // Write the received char to the specified buffer
            numBytes++;                                                                 // increase frame-length-counter 
        }
        // if no char is received, wait for timeout    
        else if(micros() - startTime >= _charTimeout) break; 
        // detect nullbyte if the next char is received below charTimeout
        else if (micros() - startTime >= _nullByteSilenece) appendNullByte = true;                           
  };

    // wait for Frame-timeout to ensure frame is complete, raise Error, if the silence-time is violated
    if((_clearRxBuffer()>0) & (receiveBuffer->getSize()!=0)){
      (numBytes>=MAXFRAMESIZE) ? raiseError(frameLengthError):raiseError(arbitrationError);
    } 

    // handle the Arbitration-Error with waiting for bus-silence
    if(getErrorState()==arbitrationError) while(_clearRxBuffer()!=0);

    // Frame received and written to specified rec-buffer
    return true;
};

// Clear the Receive-buffer 
size_t CommInterface_modbusRTU::_clearRxBuffer() {
  size_t clearedChars = 0;
  unsigned long startTime = micros();
   while (micros() - startTime < _frameTimeout) {                   // Check for a frame-timeout
    if (interface->available() > 0) {                                
      startTime = micros();                                         // update start-timestamp
      interface->read();                                            // delete one byte from rec-buffer
      clearedChars++;
    }
  };
  return clearedChars;
};

// Calculate the Timeouts
void CommInterface_modbusRTU::_calculateTimeouts(uint16_t baudrate) {
  uint8_t bitsPerChar = 11;                                   // Bits per character defined in specification
  uint16_t timePerChar = (bitsPerChar * 1000000) / baudrate;
  // Silence-time for nullbyte on bus
  _nullByteSilenece = timePerChar *0.9;                
  // Char- and frametimeout 
  if (baudrate <= 19200) {
    // Set charTimeout to 1.5 times the time to send one character
    _charTimeout = timePerChar * 1.5;

    // Set frameTimeout to 3.5 times the time to send one character
    _frameTimeout = timePerChar * 3.5;
  }
  else {
    _charTimeout = 1700;
    _frameTimeout = 4000;
  }
};


// Get the Modbus-RTU-Device-ID 
char CommInterface_modbusRTU::getDeviceId(){
  return deviceId;
};
