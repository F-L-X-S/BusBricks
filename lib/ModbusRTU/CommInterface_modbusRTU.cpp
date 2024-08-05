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
    // Serial debugging
    #ifdef DEBUG
        Serial.println("Interface started sending:");
    #endif
    
    if (sendBuffer==nullptr) return false;                        // check, if the sendbuffer-ptr is set to a valid memory
    interface->write(sendBuffer->c_str(), sendBuffer->length());  // write the byte-converted string to the interface  
    interface->flush();                                           // ensure, interfaces sending-buffer is completely empty before returning
    
    // Serial debugging
    #ifdef DEBUG
        Serial.println(*sendBuffer);
    #endif 
    return true;
};

// Receiving 
// the received Frame is directly written to the specified receive-buffer
// after a frame was received within the receive-timeout, the function returns true
// the Comm-Interfece is not checking any Content of the frame 
bool CommInterface_modbusRTU::receive(){
    // Serial debugging
    #ifdef DEBUG
        Serial.println("Interface is receiving...");
    #endif
    unsigned long startTime = micros();                             // Time, the function gets called
    uint16_t numBytes = 0;                                          // Received number of bytes
    bool receivingFlag = (deviceId == '\0') ? true:false;           // interface started receiving a frame (Slavemode: only if adressed to the device-ID, Mastermode: each frame) 

    // Wait for a relevant Frame 
    while (!interface->available()) {
    if (micros() - startTime >= _recTimeout) {
      // Serial debugging
      #ifdef DEBUG
          Serial.println("Timeout: No Frame received in specified timespan...");
      #endif
      return false;                                                 // No Frame received in specified timespan
    }
  }

    // Receive a relevant frame as long as timeout and framelength are ok
    while (micros() - startTime <= _charTimeout && interface->available() && numBytes < MAXFRAMESIZE) {
          // Serial debugging
          #ifdef DEBUG
              Serial.println("Waiting for relevant frame...");
          #endif
          if (receivingFlag || (deviceId==interface->peek()))     // check if the char in buffer is the device-ID or receiving started already
          {
              receivingFlag = true;                               // Set the receive-flag 
              startTime = micros();                               // redefine the time for measuring timeouts
              *receiveBuffer+= interface->read();                 // Write the received char to the specified buffer
              numBytes++;                                         // increase frame-length-counter 
          }else{
            interface->read();
          };
    };

    // wait for Frame-timeout to ensure frame is complete
    while (micros() - startTime < _frameTimeout);
    // Serial debugging
    #ifdef DEBUG
        Serial.println("Received:\t");
        Serial.print(*receiveBuffer);
        Serial.print("\n");
    #endif
    _clearRxBuffer();                                             // Clear the softwareSerial Rec-buffer
    
    // Serial debugging
    #ifdef DEBUG
        Serial.println("Interface has stopped receiving...");
    #endif
    
    // Frame received and written to specified rec-buffer
    return true;
};

// Clear the Receive-buffer 
void CommInterface_modbusRTU::_clearRxBuffer() {
  unsigned long startTime = micros();
   while (micros() - startTime < _frameTimeout) {                   // Check for a frame-timeout
    if (interface->available() > 0) {                                
      startTime = micros();                                         // update start-timestamp
      interface->read();                                            // delete one byte from rec-buffer
    }
  };
};

// Calculate the Timeouts
void CommInterface_modbusRTU::_calculateTimeouts(uint16_t baudrate) {
  uint8_t bitsPerChar = 11;                                   // Bits per character defined in specification
  uint16_t timePerChar = (bitsPerChar * 1000000) / baudrate;
  if (baudrate <= 19200) {
    // Set charTimeout to 1.5 times the time to send one character
    _charTimeout = timePerChar * 1.5;

    // Set frameTimeout to 3.5 times the time to send one character
    _frameTimeout = timePerChar * 3.5;
  }
  else {
    _charTimeout = 1700000;
    _frameTimeout = 4000000;
  }
};


// Get the Modbus-RTU-Device-ID 
char CommInterface_modbusRTU::getDeviceId(){
  return deviceId;
};
