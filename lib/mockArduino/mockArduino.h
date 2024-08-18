#ifndef MOCK_ARDUINO_H
#define MOCK_ARDUINO_H

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <iostream>
    #include <string>
    #include <sstream>
    #include <chrono>
    #include <queue>
    #include <functional>

    #define OUTPUT 1 ///< Define constant for OUTPUT mode
    #define INPUT 0  ///< Define constant for INPUT mode

    /**
     * @namespace arduinoMocking
     * @brief Provides mock implementations of Arduino framework functions and classes for native builds.
     */
    namespace arduinoMocking {

        /**
         * @typedef String
         * @brief Alias for `std::string` to simulate Arduino's String type.
         */
        typedef std::string String;

        /**
         * @brief Simulates the `pinMode` function for setting pin modes.
         * 
         * This function is a placeholder for the Arduino `pinMode` function and does nothing in this mock implementation.
         * 
         * @param pin The pin number to configure.
         * @param mode The mode to set for the pin (INPUT or OUTPUT).
         */
        void pinMode(int pin, int mode);

        /**
         * @class MockTime
         * @brief Simulates Arduino's time functions for testing purposes.
         * 
         * This class provides methods to get simulated time in milliseconds and microseconds.
         */
        class MockTime {
        public:
            /**
             * @brief Constructs a `MockTime` object and initializes the start time.
             */
            MockTime();

            /**
             * @brief Returns the simulated time in milliseconds.
             * 
             * @return The number of milliseconds since the start time, including any simulated time.
             */
            unsigned long millis() const;

            /**
             * @brief Returns the simulated time in microseconds.
             * 
             * @return The number of microseconds since the start time, including any simulated time.
             */
            unsigned long micros() const;

        private:
            std::chrono::steady_clock::time_point startTime; ///< Start time for the simulation
            unsigned long simulatedMillis; ///< Simulated milliseconds
            unsigned long simulatedMicros; ///< Simulated microseconds
        };

        /**
         * @brief Simulates the `millis` function to get the number of milliseconds since the program started.
         * 
         * @return The number of milliseconds since the program started.
         */
        unsigned long millis();

        /**
         * @brief Simulates the `micros` function to get the number of microseconds since the program started.
         * 
         * @return The number of microseconds since the program started.
         */
        unsigned long micros();

        /**
         * @class MockSerial
         * @brief Simulates Arduino's `Serial` class for testing purposes.
         * 
         * This class provides methods for serial communication, including print, println, and simulated input.
         */
        class MockSerial {
        public:
            /**
             * @brief Constructs a `MockSerial` object.
             */
            MockSerial();

            /**
             * @brief Begins serial communication with the specified baud rate.
             * 
             * Prints a message indicating the baud rate in the mock implementation.
             * 
             * @param baudrate The baud rate for serial communication.
             */
            void begin(unsigned long baudrate);

            /**
             * @brief Prints a string to the serial output.
             * 
             * @param str The string to print.
             */
            void print(const std::string& str);

            /**
             * @brief Prints a character to the serial output.
             * 
             * @param c The character to print.
             */
            void print(char c);

            /**
             * @brief Prints an integer to the serial output.
             * 
             * @param num The integer to print.
             */
            void print(int num);

            /**
             * @brief Prints an unsigned integer to the serial output.
             * 
             * @param num The unsigned integer to print.
             */
            void print(unsigned int num);

            /**
             * @brief Prints a long integer to the serial output.
             * 
             * @param num The long integer to print.
             */
            void print(long num);

            /**
             * @brief Prints an unsigned long integer to the serial output.
             * 
             * @param num The unsigned long integer to print.
             */
            void print(unsigned long num);

            /**
             * @brief Prints a double to the serial output.
             * 
             * @param num The double to print.
             */
            void print(double num);

            /**
             * @brief Prints a string followed by a newline to the serial output.
             * 
             * @param str The string to print.
             */
            void println(const std::string& str);

            /**
             * @brief Prints a character followed by a newline to the serial output.
             * 
             * @param c The character to print.
             */
            void println(char c);

            /**
             * @brief Prints an integer followed by a newline to the serial output.
             * 
             * @param num The integer to print.
             */
            void println(int num);

            /**
             * @brief Prints an unsigned integer followed by a newline to the serial output.
             * 
             * @param num The unsigned integer to print.
             */
            void println(unsigned int num);

            /**
             * @brief Prints a long integer followed by a newline to the serial output.
             * 
             * @param num The long integer to print.
             */
            void println(long num);

            /**
             * @brief Prints an unsigned long integer followed by a newline to the serial output.
             * 
             * @param num The unsigned long integer to print.
             */
            void println(unsigned long num);

            /**
             * @brief Prints a double followed by a newline to the serial output.
             * 
             * @param num The double to print.
             */
            void println(double num);

            /**
             * @brief Prints a newline to the serial output.
             */
            void println();

            /**
             * @brief Returns the number of bytes available to read from the serial input buffer.
             * 
             * @return The number of bytes available (size of input buffer).
             */
            int available();

            /**
             * @brief Reads a byte from the serial input buffer.
             * 
             * @return The byte read, or `-1` if no data is available.
             */
            int read();

            /**
             * @brief Writes a byte to the serial output.
             * 
             * @param byte The byte to write.
             * @return The number of bytes written (always 1 in the mock implementation).
             */
            size_t write(uint8_t byte);

            /**
             * @brief Simulates user input by adding characters to the input buffer.
             * 
             * Prompts the user to enter a string, which is added to the buffer.
             */
            void simulateInput();

            /**
             * @brief Simulates user input by adding a predefined string to the input buffer.
             * 
             * @param input The string to add to the buffer.
             */
            void simulateInput(std::string input);

        private:
            std::queue<uint8_t> inputBuffer; ///< Queue to store simulated input data.
        };

        /**
         * @var Serial
         * @brief Global instance of `MockSerial` to mimic Arduino's Serial object.
         */
        extern arduinoMocking::MockSerial Serial;

    } // namespace arduinoMocking

#endif // ARDUINO

#endif // MOCK_ARDUINO_H
