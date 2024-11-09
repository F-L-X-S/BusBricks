/**
 * @file Frame.h
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

#ifndef FRAME_H
#define FRAME_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include<Content.h>
#include<CharArray.h>

/**
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief Frame-Class as derived class from Content
The derived classes define:
-the conversion from a given payload plus protocol-specific parameters (e.g. Modbus-function-code, addresses...)
 to the frame-representation (override of content_to_rep from Content-Class)
- the extraction of protocol-specific parameters from the byte-representation (frame) 
 with eventually checking their validity (e.g. CRC) (override of rep_to_content from Content-Class
In the abstract concept of "Content" the byte-Frame itself is interpreted as the Content-Representation
The "Content"-Object represents the PDU transmitted to the services 
 *  
 * @tparam String: type of Payload of the frame ("Content")
 * @tparam CharArray: type of byte-formatted frame ("Representation")
 */
class Frame: public Content<String, CharArray>{
    public:
       /**
        * @brief Construct a new empty Frame object
        * 
        */
        Frame();

       /**
        * @brief Destroy the Frame object
        * 
        */
        ~Frame();

       /**
        * @brief Constructor for creating Frame-instance from pdu (Content)
        * 
        * @param pdu pointer to String containing the payload of the frame
        */
        Frame(String* pdu);

       /**
        * @brief Constructor for creating Frame-Object from byte-frame (Representation)
        * 
        * @param frame pointer to the char-array-object containing the byte-representation
        */
        Frame(CharArray* frame);

        /**
         * @brief Get the ServiceId, the PDU of the Frame belongs to (necessary for Service-multiplexing).
         * The Extraction of the Service-Id from the Frame is protocoll-specific and has to be implemented in the derived class.
         */
        virtual uint8_t getServiceId()=0;
};
#endif // FRAME_H