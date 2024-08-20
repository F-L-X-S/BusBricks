/**
 * @file ErrorService.h
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

#ifndef ERROR_SERVICE_H
#define ERROR_SERVICE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    #include <cstring>
    using namespace arduinoMocking;    
#endif

#include "Service.h"
#include "Error.h"

/// @brief Number of elements the services stacks can store 
#define STACKSIZE 2         

/// @brief Service-id ASCII: "e"
#define SERVICEID 0x65

/**
 * @class ErrorService
 * @brief Service class for handling Errors.
 * 
 * The `ErrorService` class manages messages of type `Message`, processes incoming messages from the receive stack, and handles sending acknowledgments. 
 * It inherits from a Service for content_type 'Error' (`Service<Error, STACKSIZE>`).
 * 
 * @section Constructors
 * - `ErrorService(uint8_t instance_id)`: Constructs a `ErrorService` with a default service ID of "e".
 * - `ErrorService(uint8_t instance_id, uint8_t service_id)`: Constructs a `ErrorService` with a custom service ID.
 * 
 * @section Methods
 * - `void stackProcessing() override`: Processes all errors from the receive stack until it is empty. Adds error-responds to the send stack and prints received errors.
 * - `void raiseError(errorCodes code)`: raises a new Error occurred internally or during receiving  
 * 
 * @private
 * - `void printError(Error* error)`: Prints the content of an error
 * 
 * @note
 * - The class handles both the processing of incoming messages and the sending of acknowledgments. 
 * - The `stackProcessing` method is responsible for managing the receive and send stacks.
 */                       
class ErrorService: public Service<Error, STACKSIZE>
{
public:
   /**
     * @brief Constructor for `ErrorService` with default Service ID "m".
     * 
     * Initializes the service with the given instance ID and a default service ID.
     * 
     * @param instance_id The instance ID for the service.
     */
    ErrorService(uint8_t instance_id);

   /**
     * @brief Constructor for `ErrorService` with a custom Service ID.
     * 
     * Initializes the service with the given instance ID and a custom service ID.
     * 
     * @param instance_id The instance ID for the service.
     * @param service_id The custom service ID.
     */
    ErrorService(uint8_t instance_id, uint8_t service_id);

    /**
     * @brief Adds an Error-object for the local Instance-Id to the Error-services rec-stack
     * 
     * @param code The error code of the error to be raised.
     */
    void raiseError(errorCodes code);

    /**
     * @brief Processes all errors from the receive stack.
     * 
     * Processes each error in the receive stack until it is empty. 
     * Each received error is printed to the serial output, 
     * generates and adds an acknowledgment PDU to the send stack, and removes the processed message from the receive stack.
     */
    void stackProcessing() override;

private:
    /**
     * @brief Prints the content of an error
     * 
     * Outputs the string representation of the error-object to the serial monitor.
     * 
     * @param message The error to be printed.
     */
    void printError(Error* error);

};

#endif // ERROR_SERVICE_H