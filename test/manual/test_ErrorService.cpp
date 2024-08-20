/**
 * @file test_ErrorService.cpp
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
#include<ErrorService.h>

int main(){
    // initialize message-service with instance-id F
    ErrorService errService('F');       
    // Foreign Instance raised error
    // Create a sample PDU for unknown Error from instance A (foreign id, error should not be broadcasted again)
    String samplePdu = "A:ERROR0";
    // add pdu to service
    errService.impart_pdu(&samplePdu);
    // start service-processes
    errService.stackProcessing();
    // check services response (foreign id, error should not be broadcasted again)
    if (errService.responseAvailable()){
        Serial.print(errService.get_response());
    }; 

    // Local Instance raised error
    // Create a sample PDU for unknown Error from instance F (local id, error should be broadcasted)
    samplePdu = "F:ERROR1";
    // add pdu to service
    errService.impart_pdu(&samplePdu);
    // start service-processes
    errService.stackProcessing();
    // check services response (foreign id, error should not be broadcasted again)
    if (errService.responseAvailable()){
        Serial.print(errService.get_response());
    }; 
    
}