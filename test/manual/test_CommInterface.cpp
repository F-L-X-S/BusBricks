/**
 * @file test_CommInterface.cpp
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

#include <CommInterface.h>
#include <Content_stack.h>

#define STACKSIZE 3

// ExmplCommInterface-Class as an exemple implementation of an CommInterface-Template Based Instance 
// with interface-type is an integer (instead of e.g. SoftwareSerial)
// The Interface is writing the Outgoing Frame to the public variable "outgoingFRame" to simulate the frame written to the bus 
// and is receiving Frames with reading the public variable "incoming Frame" each Communicationcycle
class ExmplCommInterface: public CommInterface<uint8_t>{
    private:
        // Send the Frame from Send-buffer
        bool send() override {
            if (sendBuffer!=nullptr){
                outgoingFrame = sendBuffer->getData();                        // set the outgoing frame to the value stored in the sendbuffer
                Serial.print("Sending:\t");  
                Serial.print(outgoingFrame);       
                return true;
            }
            else{
                return false;
            }
        };

        // Receive a Frame and write it to Receive-buffer
        bool receive() override {                                         
            if (incomingFrame != ""){
                Serial.print("Receiving:\t");
                Serial.print(incomingFrame);        
                // Iterate through each character in incomingFrame
                for (size_t i = 0; i < incomingFrame.length(); ++i) {
                    *receiveBuffer += char(incomingFrame[i]);  // Append each character individually
                }                                         
                return true;
            }else{
                return false;
            }            
        };

    public:
        ExmplCommInterface(): CommInterface(0, 9600){}                  // Example Interface with value 0 (only used for cycle counting), Example Baudrate of 9600 bps
        String incomingFrame = "";                                 // Simulation for an incoming Frame
        String outgoingFrame = "";                                 // Simulation for an outgoing Frame
};



// Simulate an Answering Frame from the Bus
std::string simulateAnswer(std::string outgoingFrame){
    std::string incomingFrame = "";                                     // initialize the simulated incoming frame
    if (outgoingFrame == "1nd outgoing Frame")
    {
        incomingFrame = "first incoming Answer";                        // Respond to the first frame sent
    }else if (outgoingFrame=="2nd outgoing Frame")
    {
            incomingFrame = "second incoming Answer";                   // Respond to the second frame sent
    };
    return incomingFrame;                                               // return the simulated response 
};

int main() {
    // Example Sendstack
    Content_stack<String, STACKSIZE> sendStack;
    for (size_t i = 0; i < STACKSIZE; i++)
    {
        String frame = std::to_string(i) + "nd outgoing Frame";
        sendStack.addElement(frame);
    }

    // Example Rec-Stack with 3 elements
    Content_stack<String, STACKSIZE> recStack;

    // instantiate the Communication-Interface 
    ExmplCommInterface comm_interface; 

    // initialize Sending
    CharArray send_element = *(sendStack.getElement());
    comm_interface.sendNewFrame(&send_element);

    // initialize Receiving
    CharArray rec_element;
    comm_interface.getReceivedFrame(&rec_element);

    // Simulate Communication-Cycles
    for (int i = 0; i < 10; i++)
    {
        // Print Cycle-count
        std::cout<<"\nCycle:"<<i<<std::endl;

        // Handle Sendbuffer
        if (comm_interface.finishedSending()){
            sendStack.deleteElement();                          // Delete the sent element from stack
            if (!sendStack.empty())
            {
                send_element = *(sendStack.getElement());          // Get the next Element to be sent 
                comm_interface.sendNewFrame(&send_element);     // Impart Frame that has to be sent next 
            };
            
        };

        // Handle receivebuffer
        if (comm_interface.receivedNewFrame())
        {
            recStack.addElement(rec_element.getData());               // Add the received element to the stack 
            comm_interface.getReceivedFrame(&rec_element);  // Impart memory the received item has to be stored at 
        }
        
        // Execute send-cycle
        comm_interface.sendCycle();            

        // Simulate the Response 
        comm_interface.incomingFrame = simulateAnswer(comm_interface.outgoingFrame);

        // Delete the sent Frame
        comm_interface.outgoingFrame = "";  

        // Execute rec-cycle
        comm_interface.receiveCycle();              
        
    }
    return 0;
}