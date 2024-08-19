/**
 * @file Message.h
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

#ifndef MESSAGE_H
#define MESSAGE_H

#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    #include <cstdio>
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include "Content.h"        // include Content-template

/**
 * @struct Message_content_t
 * @brief Represents the content of a message with sender and receiver information.
 * 
 * This structure is used to encapsulate a message including the sender ID, receiver ID, and the message text.
 * 
 * @var Message_content_t::sender_id
 * The ID of the sender (1 byte).
 * 
 * @var Message_content_t::receiver_id
 * The ID of the receiver (1 byte).
 * 
 * @var Message_content_t::msg_text
 * The text of the message as a `String`.
 * 
 * @note The `msg_text` is automatically null-terminated in the constructor, though it is already a `String` type which inherently handles null-termination.
 */
struct Message_content_t{
    char sender_id;                 // Src-ID[1 Byte]
    char receiver_id;               // Rec-ID[1 Byte]
    String msg_text;                // Message-Text

    /**
     * @brief Default constructor for `Message_content_t`.
     * 
     * Initializes `sender_id` and `receiver_id` to `0x0`. The `msg_text` is initialized as an empty string.
     */
    Message_content_t() : sender_id(0x0), receiver_id(0x0) {
    }

    /**
     * @brief Parameterized constructor for `Message_content_t`.
     * 
     * Initializes the `sender_id`, `receiver_id`, and `msg_text` with the provided values. The message text is appended with a null terminator.
     * 
     * @param sender_id The ID of the sender (1 byte).
     * @param receiver_id The ID of the receiver (1 byte).
     * @param msg_txt The message text to initialize.
     * @param txt_size The size of the message text (not used in this implementation but could be used for validation or other purposes).
     */
    Message_content_t(char sender_id, char receiver_id, String msg_txt, uint8_t txt_size) : 
        sender_id(sender_id), receiver_id(receiver_id), msg_text(msg_text) {
        msg_text += '\0';
    }
};

/**
 * @class Message
 * @brief Represents a message with information for sender and receiver, and provides methods for converting between PDU and message content.
 * 
 * This class handles the mapping between string-formatted PDUs (Representation) and text messages with sender- and receiver information (Content).
 * It offers constructors for creating `Message` objects from different types of inputs and provides methods to convert between a message's content 
 * and its PDU representation.
 * 
 * @note The `Message` class inherits from `Content<Message_content_t, String>`, providing a specialized implementation for handling messages.
 * 
 * @section Constructors
 * - `Message()`: Default constructor, initializes an empty message.
 * - `Message(bool boolean_expression)`: Constructs a message with default content (Sender: `0x0`, Receiver: `0x0`, Text: `\0`) if the boolean expression is `true`.
 * - `Message(String* representation)`: Constructs a message from a string-formatted PDU.
 * - `Message(Message_content_t* message_content)`: Constructs a message from `Message_content_t`, which holds the sender ID, receiver ID, and message text.
 * 
 * @section Methods
 * - `String to_string()`: Returns a string representation of the message.
 * 
 * @private
 * - `void rep_to_content()`: Converts the byte-representation (PDU) to message content. This method is overridden from the base class `Content`.
 * - `void content_to_rep()`: Converts message content to byte-representation (PDU). This method is overridden from the base class `Content`.
 */
class Message: public Content<Message_content_t, String>
{
    public:
        /**
         * @brief Default constructor for `Message`.
         * 
         * Initializes an empty message.
         */
        Message();

        /**
         * @brief Destroy the Message object
         * 
         */
        ~Message();

        /**
         * @brief Constructor for `Message` from a boolean expression.
         * 
         * Constructs a message with default content if the boolean expression is `true`.
         * 
         * @param boolean_expression A boolean value determining whether to initialize the message with default content.
         */
        Message(bool boolean_expression);

        /**
         * @brief Constructor for `Message` from a string-formatted PDU.
         * 
         * Initializes the message based on the provided string representation of the PDU.
         * 
         * @param representation A pointer to the string-formatted PDU.
         */
        Message(String* representation);

        /**
         * @brief Constructor for `Message` from `Message_content_t`.
         * 
         * Initializes the message using the provided `Message_content_t` structure.
         * 
         * @param message_content A pointer to the `Message_content_t` structure containing the message details.
         */
        Message(Message_content_t* message_content);

        /**
         * @brief Converts the message to a string representation.
         * 
         * @return A `String` representing the message.
         */
        String to_string();
        
    private:
       /**
         * @brief Converts the byte-representation (PDU) to message content.
         * 
         * This method is overridden from the base class `Content` to handle the conversion specific to `Message`.
         */
        virtual void rep_to_content() override;

        /**
         * @brief Converts message content to byte-representation (PDU).
         * 
         * This method is overridden from the base class `Content` to handle the conversion specific to `Message`.
         */
        virtual void content_to_rep() override;



};

#endif // MESSAGE_H