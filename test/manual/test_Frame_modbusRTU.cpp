#include <Frame_modbusRTU.h>

#define SLAVEID '9'
#define FUNCTIONCODE 'm'

void checkDestructor(){

    // Devicesettings 
    char slaveId = SLAVEID;
    char functionCode = FUNCTIONCODE;

    // Create a Modbus-RTU Frame from the Message-PDU 
    pduString msg_pdu = "123 Hello to the World!";
    Frame_modbusRTU frame_from_pdu(&msg_pdu, &slaveId, &functionCode);

    // Print Frame created from PDU (Content)
    std::cout<<"Frame from PDU:"<<std::endl;
    std::cout<<"Frame:\t\t"<<frame_from_pdu.getFrame()<<std::endl;
    std::cout<<"Frame-PDU:\t"<<frame_from_pdu.getPDU()<<std::endl;
    std::cout<<"Modbus-RTU Function-Code:\t\t"<<frame_from_pdu.getFunctionCode()<<std::endl;
    if (frame_from_pdu.checkCRC16()==true)
    {
        std::cout<<"CRC correct"<<std::endl;
    }else{
        std::cout<<"CRC incorrect"<<std::endl;
    }
    


    // Create a new Frame-Object from Frame above
    std::string frame = frame_from_pdu.getFrame();
    frameString frame_ptr = frame.c_str();
    Frame_modbusRTU frame_from_frame(&frame_ptr);

    // destroy frame
    frame_from_pdu.~Frame_modbusRTU();

    // Print Frame created from Frame (Representation)
    std::cout<<"Frame from Representation:"<<std::endl;
    std::cout<<"Frame:\t\t"<<frame_from_frame.getFrame()<<std::endl;
    std::cout<<"Frame-PDU:\t"<<frame_from_frame.getPDU()<<std::endl;
    std::cout<<"Modbus-RTU Function-Code:\t\t"<<frame_from_frame.getFunctionCode()<<std::endl;
    if (frame_from_frame.checkCRC16()==true)
    {
        std::cout<<"CRC correct"<<std::endl;
    }else{
        std::cout<<"CRC incorrect"<<std::endl;
    }

};

int main() {
    // Check the Destructor
    checkDestructor();

    return 0;
}