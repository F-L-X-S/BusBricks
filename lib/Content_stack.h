#ifndef CONTENT_STACK_H
#define CONTENT_STACK_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
#endif


template<typename content_class, int MaxSize>
class Content_stack{
private:
    content_class elements[MaxSize];                                     // Array for saving elements of the template                                                 
    int size;                                                           // Current number of stored elements
public: 
    Content_stack(): size(0) {}                                         // Construct Content_stack-class with zero elements

    bool addElement(const content_class& element){                       // Add element to stack                     
        if (size >= MaxSize) {
            return false;}                                              // return false, if stacksize has reached maximum 
        elements[size] = element;
        size++;
        return true;                                                    // return true, if added successful 
    };

    bool deleteElement(int index){                                      // delete element from stack   
        if (index < 0 || index >= size) {
            return false;}                                              // return false, if index of element to delete is out of range 
        // delete element, slice remaining elements, reduce size by 1
        for (int i = index; i < size - 1; ++i) {
            elements[i] = elements[i + 1];
        }
        size--;
        return true;                                                    // return true, if deleted successful
    };

    content_class getElement(int index = 0){                             // Get the element in the stack by index
        if (index >= size || size+index<0) {                            
            return false;}  

        if (index<0){
            return elements[size+index];                                // neg. index: return element indexed from end of stack (size)
        }
        return elements[index];                                         // pos. index: return element indexed from beginning of stack (0)
    };                      
};

#endif // CONTENT_STACK_H