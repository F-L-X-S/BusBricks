#include "CommInterface_modbusRTU.h"

// Construct Modbus-RTU-Communication-Interface 
CommInterface_modbusRTU::CommInterface_modbusRTU(byte rxPin, byte txPin, uint8_t baudrate) : 
    rxPin(rxPin),
    txPin(txPin),
    baudrate(baudrate),
    CommInterface<SoftwareSerial>(SoftwareSerial(rxPin, txPin), baudrate) {
};

// Setup-function
void CommInterface_modbusRTU::setup_interface(){
    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);

    // Set the baud rate for the SoftwareSerial object
    interface.begin(9600);
};