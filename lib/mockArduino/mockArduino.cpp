#include "mockArduino.h"

#ifndef ARDUINO

// Set pinmode
void arduinoMocking::pinMode(int pin, int mode) {
};

// Mock Ardiuino-time-functions
arduinoMocking::MockTime::MockTime()
    : startTime(std::chrono::steady_clock::now()), 
      simulatedMillis(0), 
      simulatedMicros(0) {

      };

// return simulated time in milliseconds
unsigned long arduinoMocking::MockTime::millis() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    return simulatedMillis + elapsed;
}

// return the simulated time in microseconds
unsigned long arduinoMocking::MockTime::micros() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();
    return simulatedMicros + elapsed;
}

// instantiate the mocked timer
arduinoMocking::MockTime mockedTimer;

// Arduino-specific Time-function:
// simulates number of milliseconds passed since the board began running the current program
unsigned long arduinoMocking::millis(){
    return mockedTimer.millis();
};

// Arduino-specific Time-function:
// simulates number of microseconds passed since the board began running the current program
unsigned long arduinoMocking::micros(){
    return mockedTimer.micros();
};



#endif

