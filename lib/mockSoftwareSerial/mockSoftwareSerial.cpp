#include "mockSoftwareSerial.h"

#ifndef ARDUINO
namespace arduinoMocking{
    SoftwareSerial::SoftwareSerial(int rx, int tx) : rxPin(rx), txPin(tx){};

    void SoftwareSerial::begin(long baud) {

    };

    void SoftwareSerial::println(const String &message) {

    };

    void SoftwareSerial::print(const String &message) {

    };


    void SoftwareSerial::write(const char* data, size_t size) {
        for (size_t i = 0; i < size; i++) {
            dataQueue.addElement(data[i]); // Add each character individually
        }
    }

    void SoftwareSerial::flush(){
        String result;
        while (!dataQueue.empty()) {
            result += *(dataQueue.getElement());
            dataQueue.deleteElement();
        }
        Serial.print("Flushed data:\t");
        Serial.print(result);
        Serial.println();
    }


    int SoftwareSerial::read() {
        if (dataQueue.empty()) {
            return -1; 
        }
        char data = *(dataQueue.getElement());
        dataQueue.deleteElement();
        return static_cast<int>(data);
    }


    int SoftwareSerial::peek() {
        if (dataQueue.empty()) {
            return -1; 
        }
        return static_cast<int>(*(dataQueue.getElement()));
    }


    int SoftwareSerial::available() {
        return (dataQueue.empty())?0:1;
    };

    // Simulate a string input by waiting for user inputs (for testing purposes)
    void SoftwareSerial::simulateInput() {
        std::string input;
        std::cout << "Enter input: ";
        std::getline(std::cin, input);

        for (char c : input) {
            dataQueue.addElement(static_cast<uint8_t>(c));
        }
    }

    // Simulate a string input (for testing purposes)
    void SoftwareSerial::simulateInput(std::string input) {
        for (char c : input) {
            dataQueue.addElement(static_cast<uint8_t>(c));
        }
    }
};
#endif // ARDUINO



