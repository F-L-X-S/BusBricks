#include "Content_stack.h"

int main() {
    Content_stack<int, 3> int_stack;
    int number = 10;
    bool reached_max = false;
    while (reached_max == false)
    {
        reached_max = !int_stack.addElement(number);
        cout<<"added element"<<std::endl;
        number ++;
    }

    return 0;
}