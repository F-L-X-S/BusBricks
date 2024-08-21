/**
 * @file Content_stack.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright * Copyright (C) 2024 Felix Schuelke
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#ifndef CONTENT_STACK_H
#define CONTENT_STACK_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
    #include <cstring>
#endif


/**
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief Content-Stack-Template
 the content-stack stores the added items (call-by-value / copy) on internal array
 the item added last is stacked on top (highest index), the one added first is stored on index "0"
 * 
 * @tparam content_class type of the items stored in the stack 
 * @tparam MaxSize maximum number of items to be stored in the stack
 */
template<typename content_class, int MaxSize>
class Content_stack{
private:
    /// @brief Array of content_class-instances 
    content_class elements[MaxSize]; 

    /// @brief Current number of stored elements                                                 
    int size;                                                           
public: 
    /**
     * @brief Construct Content_stack with zero elements
     * 
     */
    Content_stack(): size(0) {}                                         

    /**
     * @brief Destroy the Content_stack object
     * 
     */
    ~Content_stack() {} 

    /**
     * @brief Add item to stack
     * 
     * @param element Item to add to the stack 
     * @return true item added successfully 
     * @return false item not added, stacksize has reached maximum 
     */
    bool addElement(content_class element){                                          
        if (size >= MaxSize) {
            return false;}                                              // return false, if stacksize has reached maximum 
        elements[size] = element;
        size++;
        return true;                                                    // return true, if added successful 
    };

    /**
     * @brief Delete item on index-position from stack and shift all items with higher index, accepts only positive index-positions.
     * 
     * @param index index of item, that should be deleted (default 0, item on bottom of stack)
     * @return true deleted successfully
     * @return false index of element to delete is out of range 
     */
    bool deleteElement(int index=0){                                        
        if (index < 0 || index >= size) {
            return false;}                                              // return false, if index of element to delete is out of range 
        // delete element, slice remaining elements, reduce size by 1
        for (int i = index; i < size; ++i) {
            if (i==size-1) elements[i]=content_class();
            if (i<size-1) elements[i] = elements[i + 1];    
        }
        size--;
        return true;                                                    // return true, if deleted successfully
    };



    /**
     * @brief Get Pointer to element in the stack. Accepts positive and negative indexing.
     * 
     * @param index neg. index: return element indexed from end of stack (size) / pos. index: return element indexed from beginning of stack (0)
     * @return content_class* pointer to the indexed item, nullptr if no item is stored at the given index
     */
    content_class* getElement(int index = 0){                             
        if (index >= size || size+index<0) {                            
            return nullptr;}  

        if (index<0){
            return &elements[size+index];                                // neg. index: return element indexed from end of stack (size)
        }
        return &elements[index];                                         // pos. index: return element indexed from beginning of stack (0)
    };

    /**
     * @brief Check if the Stack is empty (size is 0)
     * 
     * @return true Stack is empty 
     * @return false Stack is not empty 
     */
    bool empty(){
        bool retVal = (size == 0) ? true:false;
        return retVal;
    }          

    /**
     * @brief Check if the Stack is full (size reached MaxSize)
     * 
     * @return true Stack is full 
     * @return false Stack is not full 
     */
    bool full(){
        bool retVal = (size == MaxSize) ? true:false;
        return retVal;
    }            
};

#endif // CONTENT_STACK_H