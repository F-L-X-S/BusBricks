#ifndef MOCKSOFTWARESERIAL_H
#define MOCKSOFTWARESERIAL_H


#ifdef ARDUINO
    #include "Arduino_std.h"    // import std-namesace for Arduino-build
    using namespace std;        // use std-namespace from Arduino_std
#else
    #include <iostream>
    #include <string> 
    using namespace std;
#endif

#include <Content_stack.h>

namespace arduinoMocking{

class SoftwareSerial {
public:
    SoftwareSerial(int rx, int tx);

    void begin(long baud);

    void println(const std::string &message);

    void print(const std::string &message);

    void write(char data);

    void flush();

    int available();

    int read();

    int peek();

private:
    int rxPin;
    int txPin;
    Content_stack<char, 512> dataQueue;
};

}
#endif // MOCKSOFTWARESERIAL_H