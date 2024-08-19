/**
 * @file test_Content_stack.cpp
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

#include "Content_stack.h"
#include "Message.h"

int main() {
    Content_stack<Message, 3> msg_stack;                    // initialize int-stack for max 3 elements

    // add elements till max
    Message_content_t msg_content[5];                       //initialize Message content-array for Message-constructor
    bool reached_max = false;                               // stack elements not reached max
    int i = 0;                                              // start enumeration for Message-elements
    while (reached_max == false)                            // add elements as long as the stack is not full 
    {
        // create Message-content 
        msg_content[i].sender_id = 0x11;
        msg_content[i].receiver_id = 0xFF;
        String message_text = "This is Message Number " + std::to_string(i+1);
        msg_content->msg_text = message_text;

        // create Message-element from Message-content 
        Message msg(&msg_content[i]);

        // add Message-element to stack
        reached_max = !msg_stack.addElement(msg);
        if (!reached_max)
        {
            std::cout<<"Added element:\n"<<(*(msg_stack.getElement(-1))).to_string()<<std::endl;
            i ++;
        }
    }
    std::cout<<"Reached max number of elements"<<std::endl;

    // delete elements till 0 (stack empty)
    bool reached_min = false;                                   // stack not empty
    while (!msg_stack.empty())                                  // delete elements as long as the stack is not full 
    {
        Message element_to_del = *(msg_stack.getElement(0));    // save element that should be deleted 
        msg_stack.deleteElement(0);                             // delete element on index 0 
            std::cout<<"deleted element:\n"<<element_to_del.to_string()<<std::endl;
    }
    std::cout<<"Stack empty"<<std::endl;

    return 0;
}