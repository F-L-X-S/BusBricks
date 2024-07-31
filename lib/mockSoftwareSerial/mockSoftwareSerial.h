#ifndef MOCKSOFTWARESERIAL_H
#define MOCKSOFTWARESERIAL_H


#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
    #include <Content_stack.h>

    // Namespace to redefine Arduino-Framework specific functions for native-build
    namespace arduinoMocking{

    // Class to redefine Arduino-Framework specific functions 
    // of the SoftwareSerial-Libraray for native-build
    class SoftwareSerial {
    public:
        SoftwareSerial(int rx, int tx);

        void begin(long baud);

        void println(const String &message);

        void print(const String &message);

        void write(const char* data, size_t size);

        void flush();

        int available();

        int read();

        int peek();

        // Simulate a string input defined by user (for testing purposes)
        void simulateInput();

        // Simulate a string input  defined by parameter (for testing purposes)
        void simulateInput(std::string input);

    private:
        int rxPin;
        int txPin;
        Content_stack<char, 512> dataQueue;
    };

    }
#endif
#endif // MOCKSOFTWARESERIAL_H