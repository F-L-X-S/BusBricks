#include <CommInterface.h>
#include <Content_stack.h>

class ExmplCommInterface: public CommInterface<uint8_t>{
    private:
        // initialize the interface counter as example for setup
        void setup_interface() override {
            interface = 0; 
        };


        // Send the Frame from Send-buffer
        bool send() override {
            if (sendBuffer!=nullptr){
                std::cout<<"Sending:\t"<<sendBuffer<<std::endl;        
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
    if (outgoingFrame=="first outgoing frame")
    {
        incomingFrame = "first incoming Answer";
    }else if (outgoingFrame=="second outgoing frame")
    {
            incomingFrame = "second incoming Answer";
    };
    return incomingFrame;
};

int main() {
    // Example Sendstack with 3 elements
    Content_stack<std::string, 3> sendStack;
    sendStack.addElement("first outgoing frame");
    sendStack.addElement("seconde outgoing frame");


    // Example Rec-Stack with 3 elements
    std::string* rec_element;
    Content_stack<std::string, 3> recStack;

    // instantiate the Communication-Interface 
    ExmplCommInterface comm_interface; 

    // Simulate Communication-Cycles
    for (int i = 0; i < 10; i++)
    {
        std::cout<<"\nCycle:"<<i<<std::endl;
        // only in even cycles
        if (i%2 == 0)
        {
            if (comm_interface.finishedSending()){
                sendStack.deleteElement();
                std::string element = sendStack.getElement();
                comm_interface.sendNewFrame(&element);
            };

            if (comm_interface.receivedNewFrame())
            {
                recStack.addElement(*rec_element);          // Add the received element to the stack 
                rec_element = new std::string;              // create new memory for next element 
                comm_interface.getReceivedFrame(rec_element);
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