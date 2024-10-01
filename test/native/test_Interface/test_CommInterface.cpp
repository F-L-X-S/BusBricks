/**
 * @file test_Inteface.cpp
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

#include<CommInterface.h>
#include<Content_stack.h>
#include<CharArray.h>
#include<unity.h>

// ---------------------------------------Test CommInterface---------------------------------------

// ExmplCommInterface-Class as an exemple implementation of an CommInterface-Template Based Instance 
// with interface-type is an integer (instead of e.g. SoftwareSerial)
// The Interface is writing the Outgoing Frame to the public variable "outgoingFrame" to simulate the frame written to the bus 
// and is receiving Frames with reading the public variable "incoming Frame" each Communicationcycle
class ExmplCommInterface: public CommInterface<uint8_t>{
    private:
        // Send the Frame from Send-buffer
        bool send() override {
            if (sendBuffer!=nullptr){
                outgoingFrame = sendBuffer->getHexString();           // set the outgoing frame to the value stored in the sendbuffer     
                return true;
            }
            else{
                return false;
            }
        };

        // Receive a Frame and write it to Receive-buffer
        bool receive() override {                                         
            if (incomingFrame != CharArray()){
                for (size_t i = 0; i < incomingFrame.getSize(); i++)
                {
                    *receiveBuffer += incomingFrame.getData()[i];     
                }                                               
                return true;
            }else{
                return false;
            }            
        };

    public:
        ExmplCommInterface(): CommInterface(0, 9600){}                  // Example Interface with value 0 (only used for cycle counting), Example Baudrate of 9600 bps
        CharArray incomingFrame;                                        // Simulation for an incoming Frame as a CharArray
        String outgoingFrame = "";                                      // Simulation for an outgoing Frame as hex-representation
};

// ---------------------------------------Test CommInterface Sending---------------------------------------
void test_CommInterface_sending(void) {
    #define STACKSIZE 3

    // Frame-content
    CharArray frame;

    // Example Sendstack
    Content_stack<CharArray, STACKSIZE> sendStack;
    for (size_t i = 0; i < STACKSIZE; i++)
    {
        frame = "This is Frame no. ";
        frame += '\0';                 // add nullterminator for testing (e.g. if it appears in crc)
        frame += 0xFF;                 // add example hex-value 
        frame += (char)(i+1);          // append the number of the frame
        sendStack.addElement(frame);
    }

    // instantiate the Communication-Interface 
    ExmplCommInterface comm_interface; 

    // initialize Sending
    CharArray send_element = *(sendStack.getElement());
    comm_interface.sendNewFrame(&send_element);

    // Simulate Communication-Cycles
    for (int i = 0; i < 5; i++)
    {
        // Handle Sendbuffer
        if (comm_interface.finishedSending()){
            sendStack.deleteElement();                          // Delete the sent element from stack
            if (!sendStack.empty())
            {
                send_element = *(sendStack.getElement());       // Get the next Element to be sent 
                comm_interface.sendNewFrame(&send_element);     // Impart Frame that has to be sent next 
            };
            
        };
        
        // Communicate
        comm_interface.sendCycle();            // Execute the Communication-Interfaces Send-Cycle to send 

        //Check for the excepted outgoing Frame
        if (i<STACKSIZE){
        String expectedFrame = "0x54 0x68 0x69 0x73 0x20 0x69 0x73 0x20 0x46 0x72 0x61 0x6D 0x65 0x20 0x6E 0x6F 0x2E 0x20 0x00 0xFF 0x0"
        +std::to_string(i+1); 
        TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedFrame.c_str(), comm_interface.outgoingFrame.c_str(),      
        "Outgoing Frame not as excepted");   
        }else{
        String expectedFrame = "";              // Empty Frames 
        TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedFrame.c_str(), comm_interface.outgoingFrame.c_str(),      
        "Outgoing Frame not as excepted");   
        }
        
        //Delete the sent Frame
        comm_interface.outgoingFrame = "";                      
    }
}

// ---------------------------------------Test CommInterface Receiving---------------------------------------
void test_CommInterface_receiving(void) {
    #define STACKSIZE 3

    // Frame-content
    CharArray frame;

    // Example Sendstack
    Content_stack<CharArray, STACKSIZE> exmplStack;
    for (size_t i = 0; i < STACKSIZE; i++)
    {
        frame = "This is Frame no. ";
        frame += '\0';                 // add nullterminator for testing (e.g. if it appears in crc)
        frame += 0xFF;                 // add example hex-value 
        frame += (char)(i+1);          // append the number of the frame
        exmplStack.addElement(frame);
    }

    // instantiate the Communication-Interface 
    ExmplCommInterface comm_interface; 

    // Receive-Stack to store received frames (not part of CommInterface)
    Content_stack<CharArray, STACKSIZE> recStack;

    // initialize Receiving
    CharArray rec_element;
    comm_interface.getReceivedFrame(&rec_element);

    // Simulate Communication-Cycles
    for (size_t i = 0; i < 8; i++)
    {
        // Impart the simulated received Frame
        if (!exmplStack.empty())
        {
        comm_interface.incomingFrame = *(exmplStack.getElement());
        exmplStack.deleteElement();
        };   
   
        // Communicate
        comm_interface.receiveCycle();                      // Execute the Communication-Interfaces Comm-Cycle (Send-Receive-Cycle) 
        // Handle receivebuffer
        if (comm_interface.receivedNewFrame())
        {
            recStack.addElement(rec_element);               // Add the received element to the stack 
            rec_element = CharArray();
            comm_interface.getReceivedFrame(&rec_element);  // Impart memory the received item has to be stored at 
        }

        // Reset the incoming Frame simulation
        comm_interface.incomingFrame = CharArray();

        // Check for the excepted received Frame
        if (i<STACKSIZE){
            String expectedFrame = "0x54 0x68 0x69 0x73 0x20 0x69 0x73 0x20 0x46 0x72 0x61 0x6D 0x65 0x20 0x6E 0x6F 0x2E 0x20 0x00 0xFF 0x0"
            +std::to_string(i+1);                                                                                              // Frames from ExmplFramestack-initialization 
            TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedFrame.c_str(), (*(recStack.getElement(i))).getHexString().c_str(),        // Check the Position of the added Frames in the Receive-Stack (oldest Frame on Pos 0) 
            "Received Frame not as excepted");   
        }else{    
            String expectedFrame = "0x54 0x68 0x69 0x73 0x20 0x69 0x73 0x20 0x46 0x72 0x61 0x6D 0x65 0x20 0x6E 0x6F 0x2E 0x20 0x00 0xFF 0x0"
            +std::to_string(STACKSIZE);  // last Frame received is on top of stack
            TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedFrame.c_str(), (*(recStack.getElement(-1))).getHexString().c_str(),      
            "Last Frame in Receive-Stack is not the last received Frame");   
        }
    }
}

// ---------------------------------------Run Tests---------------------------------------
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_CommInterface_sending);
    RUN_TEST(test_CommInterface_receiving);
    UNITY_END();

    return 0;
}  