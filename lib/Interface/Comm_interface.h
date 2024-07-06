#ifndef COMM_INTERFACE_H
#define COMM_INTERFACE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    using namespace std;
#endif

// Template for generic communication-interface
// The derived class defines the associations of a communication-interface (e.g. SoftwareSerial, Wire, Serial) 
// to the abstracted functions, the generic Interface-class is using
template<typename interface_type>                                       
class Comm_interface{
    private:
        interface_type interface;
        uint32_t baudrate;
    protected:
        virtual bool setup_interface();
        virtual bool write_interface();
        virtual bool read_interface();
    public:
        Comm_interface(interface_type interface, uint32_t baudrate): interface(interface), baudrate(baudrate){} 
};
#endif // COMM_INTERFACE_H