#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <iostream>
    #include <string>
    #include <chrono>
    #define OUTPUT 1
    #define INPUT 0

    namespace arduinoMocking{
        // Typedefinition for std::string
        typedef std::string String;

        // simulate setting the pinmode of the Arduino 
        void pinMode(int pin, int mode);

        // Class for simulating Arduino-specific time-functions
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
    };

#endif

#endif // MOCK_ARDUINO_H
