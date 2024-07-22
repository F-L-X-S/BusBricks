#ifndef COMMINTERFACE_MODBUSRTU
#define COMMINTERFACE_MODBUSRTU
#ifdef ARDUINO
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    using namespace std;
#endif

#include <CommInterface.h>
#include <SoftwareSerial.h>

#define SILENCE = 1750;                     // Silence-Time predefined as 1750 microseconds (for baudrates over 19500 bps)
#define TIMEOUT=750;                        // Timeout defined as 750 microseconds (for baudrates over 19500 bps)

class CommInterface_modbusRTU: public CommInterface<SoftwareSerial>{
    public:
        // Construct Communication-Interface 
        CommInterface_modbusRTU(byte rxPin, byte txPin, uint8_t baudrate);

        // Destroy Communication-Interface
        ~CommInterface_modbusRTU();

        // initialize the interface counter as example for setup
        void setup_interface() override;
    
    protected:
        // Send the Frame from Send-buffer
        bool send() override;

        // Receive a Frame and write it to Receive-buffer
        bool receive() override;

    private:
        byte rxPin;
        byte txPin;
        uint8_t baudrate;

};

#endif // COMMINTERFACE_MODBUSRTU