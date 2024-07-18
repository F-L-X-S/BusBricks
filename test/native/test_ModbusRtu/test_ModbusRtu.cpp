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

    // Create a Modbus-RTU Frame from the Message-PDU 
    pduString msg_pdu = EXMPL_PDU;
    Frame_modbusRTU frame_from_pdu(&msg_pdu, &slaveId, &functionCode);
    
    TEST_ASSERT_EQUAL_STRING_MESSAGE(EXMPL_PDU, frame_from_pdu.getPDU().c_str(),                // Check Frame PDU 
    "PDU returned from Frame-Obj does not match PDU the Frame was initialized with");    

    TEST_ASSERT_EQUAL_STRING_MESSAGE(EXMPL_FRAME, frame_from_pdu.getFrame().c_str(),            // Check Frame-Byte-Representation
    "Byte-Representation (Frame) returned from Frame-Obj does not match the expected Frame");    

    TEST_ASSERT_TRUE_MESSAGE(frame_from_pdu.checkCRC16(),                                       // Check, if the CRC-calculation for the whole Frame returns zero as CRC
    "CRC16 Rest fo whole Frame is not zero");                                                   // otherwise the CRC in the Frame is incorrect 

    // Create a new Frame-Object from Frame above
    std::string frame = frame_from_pdu.getFrame();
    frameString frame_ptr = frame.c_str();
    Frame_modbusRTU frame_from_frame(&frame_ptr);

    TEST_ASSERT_EQUAL_STRING_MESSAGE(EXMPL_PDU, frame_from_frame.getPDU().c_str(),              // Check Frame PDU 
    "PDU returned from Frame-Obj does not match PDU the Frame was initialized with");    

    TEST_ASSERT_EQUAL_STRING_MESSAGE(EXMPL_FRAME, frame_from_frame.getFrame().c_str(),          // Check Frame-Byte-Representation
    "Byte-Representation (Frame) returned from Frame-Obj does not match the expected Frame");    

    TEST_ASSERT_TRUE_MESSAGE(frame_from_frame.checkCRC16(),                                     // Check, if the CRC-calculation for the whole Frame returns zero as CRC
    "CRC16 Rest fo whole Frame is not zero");                                                   // otherwise the CRC in the Frame is incorrect 

    
}


// ---------------------------------------Run Tests---------------------------------------
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_Frame_modbusRTU);
    UNITY_END();

    return 0;
}