#ifndef CONTENT_STACK_H
#define CONTENT_STACK_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
#endif


template<typename content_type, int MaxSize>
class Content_stack{
private:
    content_type elements[MaxSize];                                     // Array for saving elements of the template                                                 
    int size;                                                           // Current number of stored elements
public: 
    Content_stack();                                                    // Construct Content_stack-class with zero elements

    bool addElement(const content_type& element);                       // Add element to stack 
    bool deleteElement(int index);                                      // delete element from stack                          
};

#endif // CONTENT_STACK_H