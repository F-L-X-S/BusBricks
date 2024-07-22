#include "CommInterface_modbusRTU.h"

// Construct Modbus-RTU-Communication-Interface 
CommInterface_modbusRTU::CommInterface_modbusRTU(byte rxPin, byte txPin, uint8_t baudrate, char deviceId = '\0') : 
    rxPin(rxPin),
    txPin(txPin),
    baudrate(baudrate),
    deviceId(deviceId),
    CommInterface<SoftwareSerial>(SoftwareSerial(rxPin, txPin), baudrate) {
        _calculateTimeouts(baudrate);
};

// Setup-function
void CommInterface_modbusRTU::setup_interface(){
    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);

    // Set the baud rate for the SoftwareSerial object
    interface.begin(baudrate);
};

// Sending
bool CommInterface_modbusRTU::send(){

};

// Receiving
bool CommInterface_modbusRTU::receive(){
    unsigned long startTime = millis();                             // Time, the function gets called
    uint16_t numBytes = 0;                                          // Received number of bytes
    bool receivingFlag = (deviceId = '\0');                         // interface started receiving a frame (Slavemode: only if adressed to the device-ID, Mastermode: each frame) 

    // Wait fo a relevant Frame 
    while (!interface.available()) {
    if (millis() - startTime >= _recTimeout) {
      return false;                                                 // No Frame received in specified timespan
    }
  }

    // Receive a relevant frame as long as timeout and framelength are ok
    while (micros() - startTime <= _charTimeout && numBytes < MAXFRAMESIZE) {
        if (interface.available()) {
            if (receivingFlag || (deviceId==interface.peek()))      // check if the char in buffer is the device-ID or receiving started already
            {
                receivingFlag = true;                               // Set the receive-flag 
                startTime = micros();                               // redefine the time for measuring timeouts
                (*receiveBuffer)[numBytes] = interface.read();      // Write the received char to the specified buffer
                numBytes++;                                         // increase frame-length-counter 
            }
        }
    };

    // wait for Frame-timeout to ensure frame is complete
    while (micros() - startTime < _frameTimeout);
    
    // Frame received and written to specified rec-buffer
    return true;
};

// Calculate the Timeouts
void CommInterface_modbusRTU::_calculateTimeouts(unsigned long baudrate) {
  unsigned long bitsPerChar = 11;                                       // Bits per character defined in specification
  unsigned long timePerChar = (bitsPerChar * 1000000) / baudrate;
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
}

