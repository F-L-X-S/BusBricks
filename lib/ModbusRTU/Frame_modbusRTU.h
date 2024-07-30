#ifndef FRAME_MODBUSRTU_H
#define FRAME_MODBUSRTU_H
#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include<Frame.h>


#define PREFIXSIZE 2    // two bytes Prefix (Device-adress, modbus-functioncode)
#define SUFFIXSIZE 2    // two bytes Suffix (CRC HighByte / LowByte)
#define CRC16MASK 0xA001
#define CRC16VALUE 0xFFFF

// Class for defining the structure of the Modbus-RTU-Frame
// Constructors:
// Construction by PDU: 
// Frame_modbusRTU(pduString* pdu, char* slaveId, char* functionCode)
// Object calculates and adds the CRC16, 
// the Modbus-Slave-ID and the Modbus-Function-Code and adds them to the Frame-representation. 
// Construction by Frame-representation:
// Frame_modbusRTU(frameString* frame)
// Object extracts Payload, Slave-ID and Function-Code and checks the CRC16 for plausibility by checking rest = 0. 
// 
// because the frame-representation is stored as a reference, the representation has to be allocated on the heap-memory (buffer)
// The allocation is done during the conversion from "Content" (construction by PDU) within the content_to_rep-function
// or it has to be initiated by calling the function "copy_to_heap" (e.g. in copy-constructor or during construction by frame-representation)
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

        // Copy constructor
        Frame_modbusRTU(const Frame_modbusRTU& other); 

        // Assignment operator
        Frame_modbusRTU& operator=(const Frame_modbusRTU& other);

        // Check the CRC16-value for teh given buffer 
        bool checkCRC16();

        // Get the Modbus-RTU-function-code of the frame-instance 
        char getFunctionCode();

    private:
        char slaveId = '\0';
        char functionCode = '\0';
        char* buffer = new char[1];

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