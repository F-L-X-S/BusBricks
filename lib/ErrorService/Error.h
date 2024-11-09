/**
 * @file Error.h
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

#ifndef ERROR_H
#define ERROR_H

#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    #include <cstdio>
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include "Content.h"        // include Content-template


/**
 * @enum errorCodes
 * @brief Enumeration for various error codes.
 * 
 * This enum defines the various error codes that can be used to represent different error states.
 * Ensure that `noError` is defined as the neutral value indicating no active error.
 */
enum errorCodes {
    /**
     * @brief no Error 
     */
    noError = '0',  

    /**
     * @brief Structure of the frame is not as expected by the services content-representation-definition
     */
    framingError = '1',   
   
    /**
     * @brief CRC checksum incorrect
     */
    crcError = '2', 
   
    /**
     * @brief Violated silence-time between frames
     */
    arbitrationError = '3',

    /**
     * @brief The Service-Id addressed by the frame was not found in the service-cluster
     * 
     */
    serviceNotFound = '4',

    /**
     * @brief Maximum framelength violated 
     * 
     */
    frameLengthError = '5',

    /**
     * @brief Send- or Receivestack reached max. size of items
     * 
     */
    overflow = '6',

    /**
     * @brief Unknown Error
     * 
     */
    unknownError = 'X'
};


/**
 * @struct ErrorContent_t
 * @brief Structure to hold an error code and its associated message.
 */
struct ErrorContent_t {
    /**
     * @brief The error code representing the specific error.
     */
    errorCodes code;

    /**
     * @brief Instance-ID if Error-Service, that raised the error 
     * 
     */
    char instanceId;

    /**
     * @brief A human-readable message describing the error.
     */
    const char* message;

    /**
     * @brief Constructor that initializes the error content based on the provided error code.
     * @param errCode The error code to initialize the structure with, unknownError by default.
     */
    ErrorContent_t(char instanceId=0x0, errorCodes errCode=noError) : 
        code(errCode), instanceId(instanceId) {
        message = getErrorMessage(errCode);
    }

private:
    /**
     * @brief Helper function to return the error message based on the error code.
     * @param code The error code for which the message is requested.
     * @return A constant character pointer to the error message string.
     */
    const char* getErrorMessage(errorCodes code) {
        switch (code) {
            case noError:
                return "No Error";
            case framingError:
                return "Framing-Error: Structure of the frame is not as expected";
            case crcError:
                return "CRC-Error: CRC checksum incorrect";
            case arbitrationError:
                return "Arbitration-Error: Silence-time between frames violated";
            case serviceNotFound:
                return "Service-not-found: Service-Id was not found in the service-cluster";
            case frameLengthError:
                return "Frame-Length-Error: Maximum length of Frame was violated";
            case overflow:
                return "Service-Stack-Overflow: Send- or Receivestack of Service has reached max. size of items";
            default:
                return "Unknown Error";
        }
    }
};

/**
 * @class Error
 * @brief Represents an error with it's error-content (error-code and an error-message), 
 * and provides methods for converting between sendable PDU and error-content.
 * 
 * This class handles the mapping between string-formatted PDUs (Representation) and errors with error-code and error-message (Content).
 * It offers constructors for creating `Error` objects from different types of inputs and provides methods to convert between a Error's content 
 * and its PDU representation.
 * 
 * @note The `Error` class inherits from `Content<ErrorContent_t, String>`, providing a specialized implementation for handling errors.
 * 
 * @section Constructors
 * - `Error()`: Default constructor, initializes an empty error.
 * - `Error(String* representation)`: Constructs an error from a string-formatted PDU.
 * - `Error(ErrorContent_t* errorContent)`: Constructs an error from `ErrorContent_t`, which holds the error-code and -message.
 * 
 * @section Methods
 * - `String to_string()`: Returns a string well formatted and printable representation of the error.
 * 
 * @private
 * - `void rep_to_content()`: Converts the byte-representation (PDU) to error content. This method is overridden from the base class `Content`.
 * - `void content_to_rep()`: Converts error content to sendable byte-representation (PDU). This method is overridden from the base class `Content`.
 */
class Error: public Content<ErrorContent_t, String>
{
    public:
        /**
         * @brief Default constructor for `Error`.
         * 
         * Initializes an empty Error object
         */
        Error();

        /**
         * @brief Destroy the Error object
         * 
         */
        ~Error();

        /**
         * @brief Constructor for `Error` from a string-formatted PDU.
         * 
         * Initializes the Error-object based on the provided string representation of the PDU.
         * 
         * @param representation A pointer to the string-formatted PDU.
         */
        Error(String* representation);

        /**
         * @brief Constructor for `Error` from `ErrorContent_t`.
         * 
         * Initializes the Error-object using the provided `ErrorContent_t` structure.
         * 
         * @param errorContent A pointer to the `ErrorContent_t` structure containing the error details.
         */
        Error(ErrorContent_t* errorContent);

        /**
         * @brief Constructor for `Error` from error-code.
         * 
         * Initializes the Error-object using the provided error-code.
         * 
         * @param code error-code 
         */
        Error(char instanceId, errorCodes code);

        /**
         * @brief Converts the Error to a well formatted, printable string.
         * 
         * @return A `String` representing the error.
         */
        String to_string();
        
    private:
       /**
         * @brief Converts the byte-representation (PDU) to error's content by initializing ErrorContent_t with Error-code.
         * 
         * This method is overridden from the base class `Content` to handle the conversion specific to `Error`.
         */
        virtual void rep_to_content() override;

        /**
         * @brief Converts error's content to byte-representation (PDU).
         * 
         * This method is overridden from the base class `Content` to handle the conversion specific to `Error`.
         */
        virtual void content_to_rep() override;



};


#endif // ERROR_H