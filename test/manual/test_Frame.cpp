/**
 * @file test_Frame.cpp
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

#include <Frame.h>
#include<Message.h>

#define PREFIXSIZE 10
#define SUFFIXSIZE 5

// Example Frame: max. 256 bytes, containing 10 times 0xFF before and after pdu
class Msg_Frame: public Frame{
    public:
    Msg_Frame(pduString* pdu) : Frame(pdu) {
        content_to_rep();
    }

    Msg_Frame(frameString* frame) : Frame(frame) {
        copy_to_heap(&representation);
        rep_to_content();
    }

    ~Msg_Frame(){
        if (representation) {
            delete[] representation;
        }        
    }

    private:
        // Convert the given Content (PDU) to Representation (Frame)
        void content_to_rep()  override {
            size_t content_length = content.length();
            size_t buffersize = PREFIXSIZE+SUFFIXSIZE+content_length + 1;
            char* buffer = new char[buffersize];
            // Fill buffer with 'F' before content
            for (size_t i = 0; i < PREFIXSIZE; ++i) {
                buffer[i] = 'F';
            }

            // Copy content into buffer
            for (size_t i = PREFIXSIZE; i < PREFIXSIZE + content_length; ++i) {
                buffer[i] = content[i - 10];
            }

            // Fill buffer with 'F' after content
            for (size_t i = PREFIXSIZE + content_length; i < buffersize - 1; ++i) {
                buffer[i] = 'F';
            }

            buffer[buffersize-1] = '\0';  // Null-terminator

            // Set representation to point to the buffer
            representation = buffer;
        };

        void rep_to_content() override {
            size_t len = strlen(representation);
            for (size_t i = PREFIXSIZE; i < len-SUFFIXSIZE; ++i) {
                content += representation[i];
            }
        };

        // Copy the representation pointing to an outside defined char-array to heap-memory
        // has to be called to ensure, that destructor is deleting from heap and not from stack
        void copy_to_heap(const char** str_ptr) {
            size_t len = strlen(*str_ptr);
            char* buffer = new char[len+1];
            for (size_t i = 0; i < len; ++i) {
                buffer[i] = (*str_ptr)[i];
            }
            buffer[len] = '\0';
            *str_ptr = buffer;
        };
    };



int main() {
    //Create a sample message-content with Sender 0x1 and Receiver 0xF
    Message_content_t msg_content;
    msg_content.sender_id = '1';
    msg_content.receiver_id = '2';
    msg_content.msg_text="Hello";   
    Message msg(&msg_content);

    // Create a generic Frame from the Message-PDU 
    pduString msg_pdu = msg.get_representation()->c_str();
    Msg_Frame msg_frame_from_pdu(&msg_pdu);

    // Print Frame created from PDU (Content)
    std::cout<<"Frame from PDU:"<<std::endl;
    std::cout<<"Frame-PDU:\t"<<msg_frame_from_pdu.getPDU()<<std::endl;
    std::cout<<"Frame:\t\t"<<msg_frame_from_pdu.getFrame()<<std::endl;

    // Create a new Frame-Object from Frame above
    std::string frame = msg_frame_from_pdu.getFrame();
    frameString frame_ptr = frame.c_str();
    Msg_Frame msg_frame_from_frame(&frame_ptr);

    // Print Frame created from Frame (Representation)
    std::cout<<"Frame from Representation:"<<std::endl;
    std::cout<<"Frame-PDU:\t"<<msg_frame_from_frame.getPDU()<<std::endl;
    std::cout<<"Frame:\t\t"<<msg_frame_from_frame.getFrame()<<std::endl;

    return 0;
}