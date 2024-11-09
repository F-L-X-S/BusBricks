/**
 * @file CommInterface_IIC.h
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

#ifndef COMMINTERFACE_IIC_H
#define COMMINTERFACE_IIC_H
#ifdef ARDUINO
    #include <Arduino.h>
    #include <Wire.h>
#else   
    #include <mockArduino.h>                // include Library to support testing with Arduino-specific functions in native-environment 
    using namespace arduinoMocking;
#endif

#include <CommInterface.h>

/// @brief debugging flag to print debugging-information on Serial 
//#define DEBUG

/// @brief maximum number of Bytes a frame can contain
#define MAXFRAMESIZE 256 

/**
 * @brief The I2C CommInterface-class specifies a communication interface with Wire as interface-type.
 *  Ensures correct data-transfer between standardized Frame-Objects in the Interface-Stack and Wire.
 * 
 */
class CommInterface_IIC: public CommInterface<TwoWire>
{
    public:
        /**
         * @brief Construct a new CommInterface_IIC object 
         * 
         * @param Wire pointer to the TwoWire-instance, the Comminterface should use (automatically instantiated by including Wire.h)
         * @param deviceId IIC-specific device-identifier 
         */
        CommInterface_IIC(TwoWire* Wire, char deviceId);

        /**
         * @brief Destroy the CommInterface_IIC object
         * 
         */
        ~CommInterface_IIC();
        
        /**
         * @brief Override of CommInterface send-function. 
         * Send the frame, sendBuffer is pointing to by writing and flushing TwoWire-interface
         * 
         * @return true frame was flushed to TwoWire-bus
         * @return false sendBuffer is nullptr
         */
        bool send() override;

        /**
         * @brief Receives a I2C-frame and writes it to `receiveBuffer`.

         * @return `true` if a frame was successfully received and written to `receiveBuffer` within the receive timeout.
         * @return `false` if no frame was received during `_recTimeout`.
         * 
         * @details The function operates in two modes:
         * - **Slave Mode**: Begins receiving only if the frame is addressed to the device's ID.
         * - **Master Mode**: Captures any incoming frame.
         * 
         * The function does not validate the content of the received frame. It uses timeouts to ensure the entire frame is received and then clears the communication interface's buffer.
         * 
         * @note If `DEBUG` is defined, debugging information is printed to the serial monitor.
         */
        bool receive() override;


        /**
         * @brief Get I2C-specific device-identifier
         * 
         * @return char I2C-specific device-identifier
         */
        char getDeviceId();

    
    private:
        /// @brief I2C-Device-ID
        const char deviceId;                    

        /// @brief time to wait for response during the receive-cycle [microseconds]      
        unsigned long _recTimeout = 1000000;     

};
#endif // COMMINTERFACE_IIC_H