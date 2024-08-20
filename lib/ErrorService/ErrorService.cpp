/**
 * @file ErrorService.cpp
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
#include "ErrorService.h"

// Constructor for creating Error-service with deafult Service-ID "e"
ErrorService::ErrorService(uint8_t instance_id): Service<Error, STACKSIZE>(SERVICEID, instance_id){}

// Constructor for creating Error-service with custom Service-ID 
ErrorService::ErrorService(uint8_t instance_id, uint8_t service_id): Service<Error, STACKSIZE>(service_id, instance_id){}


/**
 * @brief Processes all errors from the receive stack.
 * 
 * Processes each error in the receive stack until it is empty. 
 * Each received error is printed to the serial output.
 * If the Instance-ID of a received Error is the Instance-ID of the Service, 
 * the Error-representation is put to the send-stack to be broadcasted. 
 */
void ErrorService::stackProcessing() {
    // abort, if no new errors available 
    if (rec_stack.empty()) return;   

    // process the rec-stack till it's empty                                                   
    while(!rec_stack.empty()){
        // get last element from receive stack
        Error* last_error = rec_stack.getElement();
        
        // Print received Message to terminal
        printError(last_error);

        // broadcast error if raised by local host 
        if (instanceID==last_error->get_content()->instanceId)
        {
            send_stack.addElement(*last_error);
        }
               
        // delete the element the response was generated for                                           
        rec_stack.deleteElement(0);       
    }; 
}

/**
 * @brief Prints the content of an error.
 * 
 * Outputs the string representation of the error-object to the serial monitor.
 * 
 * @param error The error to be printed.
 */
void ErrorService::printError(Error* error) {
    // Convert error content to string and print it 
    Serial.println(error->to_string());
}

/**
 * @brief Adds an Error-object for the local Instance-Id and given error-code to the Error-services rec-stack
 * 
 * @param code The error code of the error to be raised.
 */
void ErrorService::raiseError(errorCodes code) {
    Error localError(instanceID,code);
    rec_stack.addElement(localError);
}