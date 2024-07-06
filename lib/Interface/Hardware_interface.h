#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    using namespace std;
#endif

// Template for generic hardware-interface
// The derived class defines the associations of a hardware-interface (e.g. SoftwareSerial, Wire, Serial) 
// to the abstracted functions, the generic Interface-class is using
template<typename interface_type>                                       
class Hardware_interface{
    private:
        interface_type interface;
        uint32_t baudrate;
    protected:
        virtual bool setup_interface();
        virtual bool write_interface();
        virtual bool read_interface();
    public:
        Hardware_interface(interface_type interface, uint32_t baudrate): interface(interface), baudrate(baudrate){} 
};
#endif // HARDWARE_INTERFACE_H