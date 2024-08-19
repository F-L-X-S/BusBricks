/**
 * @file mockSoftwareSerial.cpp
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright * Copyright (C) 2024 Felix Schuelke
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
 */


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



