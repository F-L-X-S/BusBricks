/**
 * @file mockSoftwareSerial.h
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

#ifndef MOCKSOFTWARESERIAL_H
#define MOCKSOFTWARESERIAL_H

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
    #include <Content_stack.h>

    /**
     * @namespace arduinoMocking
     * @brief Namespace to provide mock implementations of Arduino framework classes for native builds.
     */
    namespace arduinoMocking {

    /**
     * @class SoftwareSerial
     * @brief Mock class to simulate the SoftwareSerial library for native builds.
     * 
     * This class is a mock implementation of the Arduino `SoftwareSerial` class, designed for testing and simulation in non-Arduino environments.
     * It provides methods to simulate serial communication including reading, writing, and flushing data.
     * 
     * @section Public Methods
     * - `SoftwareSerial(int rx, int tx)`: Constructor to initialize the mock serial with RX and TX pins.
     * - `void begin(long baud)`: Initializes the serial communication with the specified baud rate. This method does nothing in the mock implementation.
     * - `void println(const String &message)`: Prints a message followed by a newline. This method does nothing in the mock implementation.
     * - `void print(const String &message)`: Prints a message without a newline. This method does nothing in the mock implementation.
     * - `void write(const char* data, size_t size)`: Writes data to the mock serial by adding each character to the internal queue.
     * - `void flush()`: Empties the internal queue and prints the flushed data to the console.
     * - `int available()`: Returns the number of available bytes in the internal queue (0 or 1).
     * - `int read()`: Reads a character from the internal queue. Returns `-1` if the queue is empty.
     * - `int peek()`: Returns the next character in the queue without removing it. Returns `-1` if the queue is empty.
     * - `void simulateInput()`: Prompts the user to enter a string which is then added to the internal queue. Useful for interactive testing.
     * - `void simulateInput(std::string input)`: Adds the given string to the internal queue for simulation purposes.
     * 
     * @private
     * - `int rxPin`: The RX pin number.
     * - `int txPin`: The TX pin number.
     * - `Content_stack<char, 512> dataQueue`: Internal queue to store characters for simulation.
     */
    class SoftwareSerial {
    public:
        /**
         * @brief Constructs a `SoftwareSerial` object with specified RX and TX pins.
         * 
         * @param rx The RX pin number.
         * @param tx The TX pin number.
         */
        SoftwareSerial(int rx, int tx);

        /**
         * @brief Initializes the serial communication with the specified baud rate.
         * 
         * This method does not perform any actions in the mock implementation.
         * 
         * @param baud The baud rate for the serial communication.
         */
        void begin(long baud);

        /**
         * @brief Prints a message followed by a newline.
         * 
         * This method does not perform any actions in the mock implementation.
         * 
         * @param message The message to be printed.
         */
        void println(const String &message);

        /**
         * @brief Prints a message without a newline.
         * 
         * This method does not perform any actions in the mock implementation.
         * 
         * @param message The message to be printed.
         */
        void print(const String &message);

        /**
         * @brief Writes data to the internal queue.
         * 
         * Adds each character of the provided data to the internal queue.
         * 
         * @param data Pointer to the data to be written.
         * @param size The number of bytes to write.
         */
        void write(const char* data, size_t size);

        /**
         * @brief Empties the internal queue and prints the flushed data to the console.
         * 
         * This method is used to simulate flushing the serial buffer.
         */
        void flush();

        /**
         * @brief Returns the number of available bytes in the internal queue.
         * 
         * @return The number of available bytes (0 or 1).
         */
        int available();

        /**
         * @brief Reads a character from the internal queue.
         * 
         * @return The character read from the queue, or `-1` if the queue is empty.
         */
        int read();

        /**
         * @brief Returns the next character in the queue without removing it.
         * 
         * @return The next character in the queue, or `-1` if the queue is empty.
         */
        int peek();

        /**
         * @brief Simulates user input by adding characters to the internal queue.
         * 
         * Prompts the user to enter a string, which is then added to the queue.
         */
        void simulateInput();

        /**
         * @brief Simulates user input by adding a predefined string to the internal queue.
         * 
         * @param input The string to be added to the queue.
         */
        void simulateInput(std::string input);

    private:
        int rxPin; ///< The RX pin number.
        int txPin; ///< The TX pin number.
        Content_stack<char, 512> dataQueue; ///< Internal queue to store characters for simulation.
    };

    } // namespace arduinoMocking

#endif // ARDUINO
#endif // MOCKSOFTWARESERIAL_H
