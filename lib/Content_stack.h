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
    Content_stack(): size(0) {}                          // Construct Content_stack-class with zero elements

    bool addElement(const content_type& element){                       // Add element to stack                     
        if (size >= MaxSize) {
            return false;}
        elements[size] = element;
        size++;
        return true;
    };

    bool deleteElement(int index){                                      // delete element from stack  
        if (index < 0 || index >= size) {
            return false;}
        // delete element, slice remaining elements, reduce size by 1
        for (int i = index; i < size - 1; ++i) {
            elements[i] = elements[i + 1];
        }
        size--;
        return true;
    };                        
};

#endif // CONTENT_STACK_H