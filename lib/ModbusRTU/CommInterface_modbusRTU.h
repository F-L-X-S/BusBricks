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


class CommInterface_modbusRTU: public CommInterface<SoftwareSerial>{
    public:
        // Construct Communication-Interface 
        CommInterface_modbusRTU();

        // Destroy Communication-Interface
        ~CommInterface_modbusRTU();
    
    private:
        uint16_t silenceTime = 1750;                    // Silence-Time predefined as 1750 microseconds (for baudrates over 19500 bps)
        uint16_t timeout=750;                           // Timeout defined as 750 microseconds (for baudrates over 19500 bps)

};

#endif // COMMINTERFACE_MODBUSRTU