#include "Content_stack.h"
#include "Message.h"

int main() {
    Content_stack<Message, 3> msg_stack;                    // initialize int-stack for max 3 elements

    // add elements till max
    Message_content_t msg_content;                          //initialize Message content for Message-constructor
    bool reached_max = false;                               // stack elements not reached max

    int number = 0;                                         // start enumeration for Message-elements
    while (reached_max == false)                            // add elements as long as the stack is not full 
    {
        // create Message-element
        msg_content.sender_id = 0x1;
        msg_content.receiver_id = 0xF;
        strcpy(msg_content.msg_text, "This is Message Number "+static_cast<char16_t>(number));   
        Message msg(&msg_content);

        // add Message-element to stack
        reached_max = !msg_stack.addElement(msg);
        if (!reached_max)
        {
            cout<<"Added element:\t"<<msg_stack.getElement(number).to_string()<<std::endl;
            number ++;
        }
    }
    cout<<"Reached max number of elements"<<std::endl;

    // delete elements till 0 (stack empty)
    bool reached_min = false;                               // stack not empty
    while (reached_min == false)                            // delete elements as long as the stack is not full 
    {
        Message element_to_del = msg_stack.getElement(0);
        reached_min = !msg_stack.deleteElement(0);          // delete element on index 0 
        if (!reached_min)
        {
            cout<<"deleted element: "<<element_to_del.to_string()<<std::endl;
            number ++;
        }
    }
    cout<<"Stack empty"<<std::endl;

    return 0;
}