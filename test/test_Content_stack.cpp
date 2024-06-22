#include "Content_stack.h"

int main() {
    Content_stack<int, 3> int_stack;                        // initialize int-stack for max 3 elements
    int number = 0;                                        // start enumeration by 10

    // add elements till max
    bool reached_max = false;                               // stack elements not reached max
    while (reached_max == false)                            // add elements as long as the stack is not full 
    {
        reached_max = !int_stack.addElement(number+10);
        if (!reached_max)
        {
            cout<<"added element:"<<static_cast<char32_t>(int_stack.getElement(number)) <<std::endl;
            number ++;
        }
    }
    cout<<"Reached max number of elements"<<std::endl;

    // delete elements till 0 (stack empty)
    bool reached_min = false;                               // stack not empty
    while (reached_min == false)                            // delete elements as long as the stack is not full 
    {
        int element_to_del = int_stack.getElement(0);
        reached_min = !int_stack.deleteElement(0);          // delete element on index 0 
        if (!reached_min)
        {
            cout<<"deleted element: "<<static_cast<char32_t>(element_to_del)<<std::endl;
            number ++;
        }
    }
    cout<<"Stack empty"<<std::endl;

    return 0;
}