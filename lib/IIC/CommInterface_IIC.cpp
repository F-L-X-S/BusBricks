/**
 * @file CommInterface_IIC.cpp
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

#include "CommInterface_IIC.h"

// Construct I2C-Communication-Interface with the TwoWire-Instance created by including Wire.h
CommInterface_IIC::CommInterface_IIC(TwoWire* Wire, char deviceId) : 
    deviceId(deviceId),
    CommInterface<TwoWire>(Wire) {
      Wire->begin(deviceId);          // join bus with specified ID 
    };

// Destructor
CommInterface_IIC::~CommInterface_IIC() {
}

// default-Sending-function
bool CommInterface_IIC::send(){
    if (sendBuffer==nullptr) return false;                            // check, if the sendbuffer-ptr is set to a valid memory
    const char* frame = sendBuffer->getData();                        // get start-adress of the frame to send
    uint8_t targetDeviceId = frame[0];                                // use first byte of frame as target-device-id
    interface->beginTransmission(targetDeviceId);                     // begin the transmission to the target-device
    size_t framesize = (sendBuffer->getSize() -1);                    // framesize without target-device-id
    interface->write(&frame[1], framesize);                           // write the frame without target-device-id to TwoWire-bus
    return true;
};

// Receiving 
// the received Frame is directly written to the specified receive-buffer
// after a frame was received within the receive-timeout, the function returns true
// the Comm-Interface is not checking any Content of the frame 
bool CommInterface_IIC::receive(){

};




// Get the I2C-Device-ID 
char CommInterface_IIC::getDeviceId(){
  return deviceId;
};
