#include "Content_stack.h"

int main() {
    Content_stack<int, 3> int_stack; // initialize int-stack for max 3 elements
    int number = 10; //start enumeration by 10
    bool reached_max = false;
    while (reached_max == false)    //add elements as long as the stack is not full 
    {
        reached_max = !int_stack.addElement(number);
        if (!reached_max)
        {
            cout<<"added element"<<std::endl;
            number ++;
        }
    }

    return 0;
}