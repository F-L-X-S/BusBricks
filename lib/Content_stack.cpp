#include "Content_stack.h"

// Construct Content_stack-class with zero elements
template<typename content_type, int MaxSize>
Content_stack<content_type, MaxSize>::Content_stack(): size(0) {}                                         

// Add element to stack 
template<typename content_type, int MaxSize>
bool Content_stack<content_type, MaxSize>::addElement(const content_type& element) {                      
    if (size >= MaxSize) {
        return false;
    }
    elements[size] = element;
    size++;
    return true;
}

// delete element from stack
template<typename content_type, int MaxSize>
bool Content_stack<content_type, MaxSize>::deleteElement(int index) {                                     
    if (index < 0 || index >= size) {
        return false;
    }

    // delete element, slice remaining elements, reduce size by 1
    for (int i = index; i < size - 1; ++i) {
        elements[i] = elements[i + 1];
    }
    size--;
    return true;
}