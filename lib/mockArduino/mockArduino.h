#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#ifdef ARDUINO
    #include "Arduino_std.h"    // import std-namesace for Arduino-build
    using namespace std;        // use std-namespace from Arduino_std
#else
    #include <iostream>

    #define OUTPUT 1
    #define INPUT 0

    using namespace std;
#endif




namespace arduinoMocking{
    static void pinMode(int pin, int mode);
};
#endif // MOCK_ARDUINO_H
