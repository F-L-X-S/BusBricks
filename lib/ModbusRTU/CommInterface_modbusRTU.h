#ifndef COMMINTERFACE_MODBUSRTU
#define COMMINTERFACE_MODBUSRTU
#ifdef ARDUINO
    #include <Arduino.h>
    #include <SoftwareSerial.h> 
    typedef SoftwareSerial SoftwareSerial;  // typedef for clang-build
#else   
    #include <mockSoftwareSerial.h>         // include Library to support testing with SoftwareSerial in native-environment 
    #include <mockArduino.h>                // include Library to support testing with Arduino-specific functions in native-environment 
    typedef arduinoMocking::SoftwareSerial SoftwareSerial; // typedef for clang-build
    using namespace arduinoMocking;
#endif

#include <CommInterface.h>

#define MAXFRAMESIZE 256 //Max number of Bytes a frame can contain

// The Modbus-RTU CommInterface-class specifies a communication interface with SoftwareSerial as interface-type
// Monitors modbus-specific timeouts 
// Ensures correct data-transfer between standardized Frame-Objects in the Interface-Stack and SoftwareSerial
// Does NOT inspect any frame-Content (task of the Frame-class) 
class CommInterface_modbusRTU: public CommInterface<SoftwareSerial>{
    public:
        // Construct Communication-Interface with predefined SoftwareSerial
        CommInterface_modbusRTU(SoftwareSerial* softwareserial ,uint16_t baudrate, char deviceId = '\0');

        // Destroy Communication-Interface
        ~CommInterface_modbusRTU();

        // Send the Frame from Send-buffer
        bool send() override;

        // Receive a Frame and write it to Receive-buffer
        bool receive() override;

        // Get the Modbus-Device-ID 
        char getDeviceId();

    private:
        const char deviceId;                            // Modbus-RTU Slave ID, default nullterminator for mastermode
        uint16_t _charTimeout;                          // Timeout between two chars received 
        uint16_t _frameTimeout;                         // min time between two Frames
        uint16_t _recTimeout = 100;                     // time to wait for response during the receive-cycle

        void _calculateTimeouts(uint16_t baud);         // Calculate all timeouts based on the specified baudrate
        void _clearRxBuffer();                          // Clear the receive-buffer 

};

#endif // COMMINTERFACE_MODBUSRTU