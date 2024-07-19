#ifndef CONTENT_STACK_H
#define CONTENT_STACK_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
#endif


template<typename content_class, int MaxSize>
class Content_stack{
private:
    content_class elements[MaxSize];                                    // Array for saving elements of the template                                                 
    int size;                                                           // Current number of stored elements
public: 
    // Construct Content_stack-class with zero elements
    Content_stack(): size(0) {}                                         

    // Destroy Content-stack
    ~Content_stack() {} 

    // Add element to stack
    // True if successful
    bool addElement(content_class element){                                          
        if (size >= MaxSize) {
            return false;}                                              // return false, if stacksize has reached maximum 
        elements[size] = element;
        size++;
        return true;                                                    // return true, if added successful 
    };

    // Delete element on index-position from stack 
    // only positive index-positions
    // True if successful
    bool deleteElement(int index=0){                                        
        if (index < 0 || index >= size) {
            return false;}                                              // return false, if index of element to delete is out of range 
        // delete element, slice remaining elements, reduce size by 1
        for (int i = index; i < size - 1; ++i) {
            elements[i] = elements[i + 1];
        }
        size--;
        return true;                                                    // return true, if deleted successful
    };

    // Get the element in the stack by index: 
    // neg. index: return element indexed from end of stack (size)
    // pos. index: return element indexed from beginning of stack (0)
    // True if successful
    content_class getElement(int index = 0){                             
        if (index >= size || size+index<0) {                            
            return false;}  

        if (index<0){
            return elements[size+index];                                // neg. index: return element indexed from end of stack (size)
        }
        return elements[index];                                         // pos. index: return element indexed from beginning of stack (0)
    };

    // Check if the Stack is empty 
    bool empty(){
        if (size == 0){
            return true;
        }
        return false;
    }                      
};

#endif // CONTENT_STACK_H