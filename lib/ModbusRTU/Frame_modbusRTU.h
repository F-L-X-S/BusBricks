#ifndef FRAME_MODBUSRTU_H
#define FRAME_MODBUSRTU_H
#ifdef ARDUINO
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    using namespace std;
#endif

#include<Frame.h>


#define PREFIXSIZE 2    // two bytes Prefix (Device-adress, modbus-functioncode)
#define SUFFIXSIZE 2    // two bytes Suffix (CRC HighByte / LowByte)
#define CRC16MASK 0xA001
#define CRC16VALUE 0xFFFF

// Class for defining the structure of the Modbus-RTU-Frame
// Constructors:
// Frame_modbusRTU(pduString* pdu, char* slaveId, char* functionCode)
// Frame_modbusRTU(frameString* frame)
class Frame_modbusRTU: public Frame{
    public:
        // Construct Modbus-RRU-Frame from given PDU (Content)
        Frame_modbusRTU(pduString* pdu, char* slaveId, char* functionCode);

        // Construct Modbus-RTU-Frame from given Byte-Frame (Representation)
        Frame_modbusRTU(frameString* frame);

        // Construct empty Modbus-RTU-Frame 
        Frame_modbusRTU();

        // Deconstructor 
        ~Frame_modbusRTU();

        // Check the CRC16-value for teh given buffer 
        bool checkCRC16();

        // Get the Modbus-RTU-function-code of the frame-instance 
        char getFunctionCode();

    private:
        char slaveId = '\0';
        char functionCode = '\0';

        // Convert the given Content (PDU) to Representation (Frame)
        void content_to_rep()  override;

        // Convert the given Representation (Frame) to Content (PDU) 
        void rep_to_content() override;

        // Calculate the CRC16-value of the given buffer 
        unsigned short calcCRC16(char* buffer, uint8_t size);

        // Copy the representation pointing to an outside defined char-array to heap-memory
        // has to be called to ensure, that destructor is deleting from heap and not from stack
        void copy_to_heap(const char** str_ptr);

    };


#endif // FRAME_MODBUSRTU_H