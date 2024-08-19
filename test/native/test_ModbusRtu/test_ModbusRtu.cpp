/**
 * @file test_ModbusRtu.cpp
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
#include<unity.h>

#define SLAVEID '9'
#define FUNCTIONCODE 'm'
#define EXMPL_PDU "123 Hello to the World!"                     
#define EXMPL_FRAME "9m123 Hello to the World!\xA5\x82"                                 // Frame with calculated CRC 

// ---------------------------------------Test Modbus-RTU Frame---------------------------------------
void test_Frame_modbusRTU(void) {
    // Devicesettings 
    char slaveId = SLAVEID;
    char functionCode = FUNCTIONCODE;

    // Check empty Frame 
    Frame_modbusRTU* framePtr = new  Frame_modbusRTU();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("", (framePtr->getPDU()).c_str(),                  // Check empty Frame  
    "PDU returned from empty Frame-Obj is not empty");   

    TEST_ASSERT_EQUAL_STRING_MESSAGE("", (framePtr->getFrame()).c_str(),                // Check Frame-Byte-Representation
    "Byte-Representation (Frame) returned from empty Frame-Obj is not empty");    

    delete framePtr;                                                                    // Call Destructor (test-error if failed)

    // Create a Modbus-RTU Frame from the Message-PDU 
    pduString msg_pdu = EXMPL_PDU;
    Frame_modbusRTU frame_from_pdu(&msg_pdu, &slaveId, &functionCode);
    
    TEST_ASSERT_EQUAL_STRING_MESSAGE(EXMPL_PDU, frame_from_pdu.getPDU().c_str(),                // Check Frame PDU 
    "PDU returned from Frame-Obj created by frame-representation does not match PDU the Frame was initialized with");    

    TEST_ASSERT_EQUAL_STRING_MESSAGE(EXMPL_FRAME, frame_from_pdu.getFrame().c_str(),            // Check Frame-Byte-Representation
    "Byte-Representation (Frame) returned from Frame-Obj created by PDU does not match the expected Frame");    

    TEST_ASSERT_TRUE_MESSAGE(frame_from_pdu.checkCRC16(),                                       // Check, if the CRC-calculation for the whole Frame returns zero as CRC
    "CRC16 Rest fo whole Frame is not zero");                                                   // otherwise the CRC in the Frame is incorrect 

    // Create a new Frame-Object from Frame above
    std::string frame = frame_from_pdu.getFrame();
    frameString frame_ptr = frame.c_str();
    Frame_modbusRTU frame_from_frame(&frame_ptr);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(EXMPL_PDU, frame_from_frame.getPDU().c_str(),              // Check Frame PDU 
    "PDU returned from Frame-Obj created by frame-representation does not match PDU the Frame was initialized with");    

    TEST_ASSERT_EQUAL_STRING_MESSAGE(EXMPL_FRAME, frame_from_frame.getFrame().c_str(),          // Check Frame-Byte-Representation
    "Byte-Representation (Frame) returned from Frame-Obj created by frame-representation does not match the expected Frame");    

    TEST_ASSERT_TRUE_MESSAGE(frame_from_frame.checkCRC16(),                                     // Check, if the CRC-calculation for the whole Frame returns zero as CRC
    "CRC16 Rest fo whole Frame created by frame-representation is not zero");                                                   // otherwise the CRC in the Frame is incorrect 

}


// ---------------------------------------Run Tests---------------------------------------
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_Frame_modbusRTU);
    UNITY_END();

    return 0;
}