#include <CommInterface.h>
#include <Content_stack.h>

#define STACKSIZE 3

class ExmplCommInterface: public CommInterface<uint8_t>{
    private:
        // initialize the interface counter as example for setup
        void setup_interface() override {
            interface = 0; 
        };


        // Send the Frame from Send-buffer
        bool send() override {
            if (sendBuffer!=nullptr){
                outgoingFrame = *sendBuffer;
                std::cout<<"Sending:\t"<<outgoingFrame<<std::endl;        
                return true;
            }
            else{
                return false;
            }
        };

        // Receive a Frame and write it to Receive-buffer
        bool receive() override {                                         
            if (incomingFrame != ""){
                std::cout<<"Receiving:\t"<<incomingFrame<<std::endl;           
                *receiveBuffer = incomingFrame;
                incomingFrame = "";
                return true;
            }else{
                return false;
            }            
        };

    public:
        ExmplCommInterface(): CommInterface(0, 9600){}                  // Example Interface with value 0 (only used for cycle counting), Example Baudrate of 9600 bps
        std::string incomingFrame = "";                                     // Simulation for an incoming Frame
        std::string outgoingFrame = "";                                     // Simulation for an outgoing Frame

};



// Simulate an Answering Frame from the Bus
std::string simulateAnswer(std::string outgoingFrame){
    std::string incomingFrame = "";
    if (outgoingFrame == "1nd outgoing Frame")
    {
        incomingFrame = "first incoming Answer";
    }else if (outgoingFrame=="2nd outgoing Frame")
    {
            incomingFrame = "second incoming Answer";
    };
    return incomingFrame;
};

int main() {
    // Example Sendstack
    Content_stack<std::string, STACKSIZE> sendStack;
    for (size_t i = 0; i < STACKSIZE; i++)
    {
        std::string frame = std::to_string(i) + "nd outgoing Frame";
        sendStack.addElement(frame);
    }

    // Example Rec-Stack with 3 elements
    Content_stack<std::string, STACKSIZE> recStack;

    // instantiate the Communication-Interface 
    ExmplCommInterface comm_interface; 

    // initialize Sending
    std::string element = sendStack.getElement();
    comm_interface.sendNewFrame(&element);

    // initialize Receiving
    std::string rec_element;
    comm_interface.getReceivedFrame(&rec_element);

    // Simulate Communication-Cycles
    for (int i = 0; i < 10; i++)
    {
        std::cout<<"\nCycle:"<<i<<std::endl;
        // only in even cycles
        if (i%2 == 0)
        {
            if (comm_interface.finishedSending()){
                sendStack.deleteElement();
                if (!sendStack.empty())
                {
                    element = sendStack.getElement();
                };
                comm_interface.sendNewFrame(&element);
            };

            if (comm_interface.receivedNewFrame())
            {
                recStack.addElement(rec_element);          // Add the received element to the stack 
                comm_interface.getReceivedFrame(&rec_element);
            }
            
            comm_interface.incomingFrame = simulateAnswer(comm_interface.outgoingFrame);
        };


        // Communicate
        comm_interface.execCommunicationCycle();
        if (comm_interface.outgoingFrame != "")
        {
            std::cout<<"Outgoing Frame:"<<comm_interface.outgoingFrame<<std::endl;
        };    
        
    }
    return 0;
}