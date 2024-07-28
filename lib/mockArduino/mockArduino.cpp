#include "mockArduino.h"

#ifndef ARDUINO

namespace arduinoMocking{

// Set pinmode
void pinMode(int pin, int mode) {
};

// --------------------------- Arduino-specific time-functions ---------------------------
// Mock Ardiuino-time-functions
MockTime::MockTime()
    : startTime(std::chrono::steady_clock::now()), 
      simulatedMillis(0), 
      simulatedMicros(0) {

      };

// return simulated time in milliseconds
unsigned long MockTime::millis() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
    return simulatedMillis + elapsed;
}

// return the simulated time in microseconds
unsigned long MockTime::micros() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();
    return simulatedMicros + elapsed;
}

// instantiate the mocked timer
MockTime mockedTimer;

// Arduino-specific Time-function:
// simulates number of milliseconds passed since the board began running the current program
unsigned long millis(){
    return mockedTimer.millis();
};

// Arduino-specific Time-function:
// simulates number of microseconds passed since the board began running the current program
unsigned long micros(){
    return mockedTimer.micros();
};

// --------------------------- Arduino-specific Serial-functions ---------------------------
MockSerial::MockSerial(){
};

// Begin the serial communication
void MockSerial::begin(unsigned long baudrate) {
    std::cout << "Serial started at " << baudrate << " baud" << std::endl;
}

// Print functions
void MockSerial::print(const std::string& str) {
    std::cout << str;
}

void MockSerial::print(char c) {
    std::cout << c;
}

void MockSerial::print(int num) {
    std::cout << num;
}

void MockSerial::print(unsigned int num) {
    std::cout << num;
}

void MockSerial::print(long num) {
    std::cout << num;
}

void MockSerial::print(unsigned long num) {
    std::cout << num;
}

void MockSerial::print(double num) {
    std::cout << num;
}

// Println functions
void MockSerial::println(const std::string& str) {
    std::cout << str << std::endl;
}

void MockSerial::println(char c) {
    std::cout << c << std::endl;
}

void MockSerial::println(int num) {
    std::cout << num << std::endl;
}

void MockSerial::println(unsigned int num) {
    std::cout << num << std::endl;
}

void MockSerial::println(long num) {
    std::cout << num << std::endl;
}

void MockSerial::println(unsigned long num) {
    std::cout << num << std::endl;
}

void MockSerial::println(double num) {
    std::cout << num << std::endl;
}

void MockSerial::println() {
    std::cout << std::endl;
}

// Read functions
int MockSerial::available() {
    return inputBuffer.size();
}

int MockSerial::read() {
    if (inputBuffer.empty()) {
        return -1; // No data available
    }
    uint8_t byte = inputBuffer.front();
    inputBuffer.pop();
    return byte;
}

// Write functions
size_t MockSerial::write(uint8_t byte) {
    std::cout << byte;
    return 1; // Indicate that one byte was written
}

// Simulate a string input by waiting for user inputs (for testing purposes)
void MockSerial::simulateInput() {
    std::string input;
    std::cout << "Enter input: ";
    std::getline(std::cin, input);

    for (char c : input) {
        inputBuffer.push(static_cast<uint8_t>(c));
    }
    // Simulate the Enter key (newline character)
    inputBuffer.push('\n');
}

// Simulate a string input (for testing purposes)
void MockSerial::simulateInput(std::string input) {
    for (char c : input) {
        inputBuffer.push(static_cast<uint8_t>(c));
    }
}


// Instantiate the mocked Serial object
MockSerial Serial;

}
#endif

