#ifndef COMMINTERFACE_MODBUSRTU
#define COMMINTERFACE_MODBUSRTU
#ifdef ARDUINO
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    using namespace std;
#endif

#include <SoftwareSerial.h>
#include <CommInterface.h>

#define MAXFRAMESIZE 256 //Max number of Bytes a frame can contain

// The Modbus-RTU CommInterface-class specifies a communication interface with SoftwareSerial as interface-type
// Monitors modbus-specific timeouts 
// Ensures correct data-transfer between standardized Frame-Objects in the Interface-Stack and SoftwareSerial
// Does NOT inspect any frame-Content (task of the Frame-class) 
class CommInterface_modbusRTU: public CommInterface<SoftwareSerial>{
    public:
        // Construct Communication-Interface 
        CommInterface_modbusRTU(byte rxPin, byte txPin, uint8_t baudrate, char deviceId = '\0');

        // Destroy Communication-Interface
        ~CommInterface_modbusRTU();

        // Execute the Communication-Cycle
        void execCommunicationCycle();

        // initialize the interface counter as example for setup
        void setup_interface() override;

        // Send the Frame from Send-buffer
        bool send() override;

        // Receive a Frame and write it to Receive-buffer
        bool receive() override;

        // Get the Modbus-Device-ID 
        char getDeviceId();

    private:
        byte rxPin;                                     // Receive-Pin 
        byte txPin;                                     // Transmit-Pin
        const char deviceId;                                  // Modbus-RTU Slave ID, default nullterminator for mastermode
        unsigned long baudrate;                         // Baudrate 
        unsigned long _charTimeout;                     // Timeout between two chars received 
        unsigned long _frameTimeout;                    // min time between two Frames
        unsigned long _recTimeout = 100;                // time to wait for response during the receive-cycle

        void _calculateTimeouts(unsigned long baud);    // Calculate all timeouts based on the specified baudrate
        void _clearRxBuffer();                          // Clear the receive-buffer 

};

#endif // COMMINTERFACE_MODBUSRTU