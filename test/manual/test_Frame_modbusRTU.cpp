/**
 * @file test_Frame_modbusRTU.cpp
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

#include <Frame_modbusRTU.h>

#define SLAVEID '9'
#define FUNCTIONCODE 'm'

void printFrame(Frame_modbusRTU* frame){
    std::cout<<"Frame:\t\t"<<frame->getFrame()<<std::endl;
    std::cout<<"Frame-PDU:\t"<<frame->getPDU()<<std::endl;
    std::cout<<"Modbus-RTU Function-Code:\t\t"<<frame->getFunctionCode()<<std::endl;
    if (frame->checkCRC16()==true)
    {
        std::cout<<"CRC correct"<<std::endl;
    }else{
        std::cout<<"CRC incorrect"<<std::endl;
    }
};

void checkDestructor(){

    // Devicesettings 
    char slaveId = SLAVEID;
    char functionCode = FUNCTIONCODE;

    // Create a Modbus-RTU Frame from the Message-PDU 
    pduString msg_pdu = "123 Hello to the World!";
    Frame_modbusRTU frame_from_pdu(&msg_pdu, &slaveId, &functionCode);

    // Print Frame created from PDU (Content)
    std::cout<<"Frame from PDU:"<<std::endl;
    printFrame(&frame_from_pdu);
    
    // Create a new Frame-Object from Frame above
    std::string frame = frame_from_pdu.getFrame();
    frameString frame_ptr = frame.c_str();
    Frame_modbusRTU frame_from_frame(&frame_ptr);

    // Print Frame created from Frame (Representation)
    std::cout<<"Frame from Representation:"<<std::endl;
    printFrame(&frame_from_frame);

    // Create a new empty Frame 
    Frame_modbusRTU assignedFrame;
    std::cout<<"empty Frame:"<<std::endl;
    printFrame(&assignedFrame);

    // Check assignment
    assignedFrame = frame_from_pdu;
    std::cout<<"Frame from Assignment:"<<std::endl;
    printFrame(&assignedFrame);

    // Create a new Frame by copying 
    Frame_modbusRTU copiedFrame = frame_from_pdu;
    std::cout<<"Frame from Copy:"<<std::endl;
    printFrame(&copiedFrame);

};

int main() {
    // Check the Destructor
    checkDestructor();

    return 0;
}