#include<CommInterface.h>
#include<Content_stack.h>
#include<unity.h>

// ---------------------------------------Test CommInterface---------------------------------------

// ExmplCommInterface-Class as an exemple implementation of an CommInterface-Template Based Instance 
// with interface-type is an integer (instead of e.g. SoftwareSerial)
// The Interface is writing the Outgoing Frame to the public variable "outgoingFRame" to simulate the frame written to the bus 
// and is receiving Frames with reading the public variable "incoming Frame" each Communicationcycle
class ExmplCommInterface: public CommInterface<uint8_t>{
    private:
        // Send the Frame from Send-buffer
        bool send() override {
            if (sendBuffer!=nullptr){
                outgoingFrame = *sendBuffer;                            // set the outgoing frame to the value stored in the sendbuffer     
                return true;
            }
            else{
                return false;
            }
        };

        // Receive a Frame and write it to Receive-buffer
        bool receive() override {                                         
            if (incomingFrame != ""){       
                *receiveBuffer = incomingFrame;                         // copy the value of the incoming frame to the receivebuffer                               
                return true;
            }else{
                return false;
            }            
        };

    public:
        ExmplCommInterface(): CommInterface(0, 9600){}                  // Example Interface with value 0 (only used for cycle counting), Example Baudrate of 9600 bps
        std::string incomingFrame = "";                                 // Simulation for an incoming Frame
        std::string outgoingFrame = "";                                 // Simulation for an outgoing Frame
};

// ---------------------------------------Test CommInterface Sending---------------------------------------
void test_CommInterface_sending(void) {
    #define STACKSIZE 3
    std::string frame;
    // Example Sendstack
    Content_stack<std::string, STACKSIZE> sendStack;
    for (size_t i = 0; i < STACKSIZE; i++)
    {
        frame = "outgoing Frame no. "+std::to_string(i);    // initialize Sendstack with 3 exampleframes 
        sendStack.addElement(frame);
    }

    // instantiate the Communication-Interface 
    ExmplCommInterface comm_interface; 

    // initialize Sending
    std::string send_element = *(sendStack.getElement());
    comm_interface.sendNewFrame(&send_element);

    // Simulate Communication-Cycles
    for (int i = 0; i < 5; i++)
    {
        // Handle Sendbuffer
        if (comm_interface.finishedSending()){
            sendStack.deleteElement();                          // Delete the sent element from stack
            if (!sendStack.empty())
            {
                send_element = *(sendStack.getElement());          // Get the next Element to be sent 
                comm_interface.sendNewFrame(&send_element);     // Impart Frame that has to be sent next 
            };
            
        };
        
        // Communicate
        comm_interface.execCommunicationCycle();            // Execute the Communication-Interfaces Comm-Cycle (Send-Receive-Cycle)

        //Check for the excepted outgoing Frame
        if (i<STACKSIZE){
        TEST_ASSERT_EQUAL_STRING_MESSAGE((*(sendStack.getElement())).c_str(), comm_interface.outgoingFrame.c_str(),      
        "Outgoing Frame not as excepted");   
        }else{
        frame = "";                                         // Empty Frames 
        TEST_ASSERT_EQUAL_STRING_MESSAGE(frame.c_str(), comm_interface.outgoingFrame.c_str(),      
        "Outgoing Frame not as excepted");   
        }
        
        //Delete the sent Frame
        comm_interface.outgoingFrame = "";                      
    }
}

// ---------------------------------------Test CommInterface Receiving---------------------------------------
void test_CommInterface_receiving(void) {
    #define STACKSIZE 3
    std::string frame;
    // Example Frames to be received
    Content_stack<std::string, STACKSIZE> exmplFrameStack;
    for (size_t i = 0; i < STACKSIZE; i++)
    {
        frame = "outgoing Frame no. "+std::to_string(i);    // initialize Stack with 3 exampleframes 
        exmplFrameStack.addElement(frame);
    }

    // Example Rec-Stack with 3 elements
    Content_stack<std::string, STACKSIZE> recStack;

    // instantiate the Communication-Interface 
    ExmplCommInterface comm_interface; 

    // initialize Receiving
    std::string rec_element;
    comm_interface.getReceivedFrame(&rec_element);

    // Simulate Communication-Cycles
    for (int i = 0; i < 8; i++)
    {
        // Impart the simulated received Frame
        if (!exmplFrameStack.empty())
        {
        comm_interface.incomingFrame = *(exmplFrameStack.getElement());
        exmplFrameStack.deleteElement();
        };   
   
        // Communicate
        comm_interface.execCommunicationCycle();            // Execute the Communication-Interfaces Comm-Cycle (Send-Receive-Cycle)

        // Handle receivebuffer
        if (comm_interface.receivedNewFrame())
        {
            recStack.addElement(rec_element);               // Add the received element to the stack 
            comm_interface.getReceivedFrame(&rec_element);  // Impart memory the received item has to be stored at 
        }

        // Reset the incoming Frame simulation
        comm_interface.incomingFrame = "";

        // Check for the excepted received Frame
        if (i<STACKSIZE){
            frame = "outgoing Frame no. "+std::to_string(i);          // Frames from ExmplFramestack-initialization 
            TEST_ASSERT_EQUAL_STRING_MESSAGE(frame.c_str(), (*(recStack.getElement(i))).c_str(),     // Check the Position of the added Frames in the Receive-Stack (oldest Frame on Pos 0) 
            "Received Frame not as excepted");   
        }else{    
            frame = "outgoing Frame no. "+std::to_string(0);          // First Frame received is on top of stack
            TEST_ASSERT_EQUAL_STRING_MESSAGE(frame.c_str(), (*(recStack.getElement())).c_str(),      
            "Last Frame in Receive-Stack not as excepted after Stack is full and no Frames were received");   
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