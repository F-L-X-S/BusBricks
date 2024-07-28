#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <iostream>
    #include <string>
    #include <sstream>
    #include <chrono>
    #define OUTPUT 1
    #define INPUT 0

    namespace arduinoMocking{
        // Typedefinition for std::string
        typedef std::string String;

        // simulate setting the pinmode of the Arduino 
        void pinMode(int pin, int mode);

        // --------------------------- Arduino-specific time-functions ---------------------------
        class MockTime {
            public:
                // constructor
                MockTime();

                // return simulated time in milliseconds
                unsigned long millis() const;

                // return simulated time in microseconds
                unsigned long micros() const;

            private:
                // starttime
                std::chrono::steady_clock::time_point startTime;
                
                unsigned long simulatedMillis;
                
                unsigned long simulatedMicros;
        };

        // Arduino-specific Time-function:
        // simulates number of milliseconds passed since the board began running the current program
        unsigned long millis();

        // Arduino-specific Time-function:
        // simulates number of microseconds passed since the board began running the current program
        unsigned long micros();

        // --------------------------- Arduino-specific Serial-functions ---------------------------
            class MockSerial {
            public:
                // Constructor 
                MockSerial();

                // Begin the serial communication
                void begin(unsigned long baudrate);

                // Print functions
                void print(const std::string& str);

                void print(char c);

                void print(int num);

                void print(unsigned int num);

                void print(long num);

                void print(unsigned long num);

                void print(double num);

                // Println functions
                void println(const std::string& str);

                void println(char c);

                void println(int num);

                void println(unsigned int num);

                void println(long num);

                void println(unsigned long num);

                void println(double num);

                void println();

                // Read functions
                int available();

                int read();

                // Write functions
                size_t write(uint8_t byte);

                // Simulate a string input (for testing purposes)
                void simulateInput(const std::string& input);
        };

        // Global instance to mimic the Arduino environment
        extern arduinoMocking::MockSerial Serial;
    };

#endif

#endif // MOCK_ARDUINO_H
