/**
 * @file Message_service.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MESSAGE_SERVICE_H
#define MESSAGE_SERVICE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    #include <cstring>
    using namespace arduinoMocking;    
#endif

#include "Service.h"
#include "Message.h"

/// @brief Number of elements the services stacks can store 
#define STACKSIZE 2         

/// @brief Service-id ASCII: "m"
#define FUNCTIONCODE 0x6D

/**
 * @class Message_service
 * @brief Service class for handling messages and acknowledgments.
 * 
 * The `Message_service` class manages messages of type `Message`, processes incoming messages from the receive stack, and handles sending acknowledgments. 
 * It inherits from a Service for conetnt_type 'Message' (`Service<Message, STACKSIZE>`).
 * 
 * @section Constructors
 * - `Message_service(uint8_t instance_id)`: Constructs a `Message_service` with a default service ID of "m".
 * - `Message_service(uint8_t instance_id, uint8_t service_id)`: Constructs a `Message_service` with a custom service ID.
 * 
 * @section Methods
 * - `void stackProcessing() override`: Processes all messages from the receive stack until it is empty. Adds acknowledgments to the send stack and prints received messages.
 * - `uint8_t get_destinationId()`: Retrieves the destination instance ID for the current response PDU.
 * - `void sendMessage(char receiverId, String messagetext)`: Sends a new message with the specified receiver ID and message text.
 * 
 * @private
 * - `void sendAck(Message* message)`: Adds an acknowledgment for the given message to the send stack.
 * - `void printMessage(Message* message)`: Prints the content of the received message.
 * 
 * @note
 * - The class handles both the processing of incoming messages and the sending of acknowledgments. 
 * - The `stackProcessing` method is responsible for managing the receive and send stacks, ensuring messages are properly handled and acknowledged.
 */                       
class Message_service: public Service<Message, STACKSIZE>
{
public:
   /**
     * @brief Constructor for `Message_service` with default Service ID "m".
     * 
     * Initializes the service with the given instance ID and a default service ID.
     * 
     * @param instance_id The instance ID for the service.
     */
    Message_service(uint8_t instance_id);

   /**
     * @brief Constructor for `Message_service` with a custom Service ID.
     * 
     * Initializes the service with the given instance ID and a custom service ID.
     * 
     * @param instance_id The instance ID for the service.
     * @param service_id The custom service ID.
     */
    Message_service(uint8_t instance_id, uint8_t service_id);

    /**
     * @brief Processes all messages from the receive stack.
     * 
     * Processes each message in the receive stack until it is empty. For each message, it prints the message content to the serial output, 
     * generates and adds an acknowledgment PDU to the send stack, and removes the processed message from the receive stack.
     */
    void stackProcessing() override;

    /**
     * @brief Retrieves the destination instance ID for the current response PDU in the send stack.
     * 
     * @return uint8_t The destination instance ID extracted from the message in the send stack.
     */
    uint8_t get_destinationId();

    /**
     * @brief Sends a new message.
     * 
     * Constructs and sends a message with the specified receiver ID and message text. 
     * The message is added to the send stack for transmission.
     * 
     * @param receiverId char, The ID of the message receiver.
     * @param messagetext String, The text of the message to be sent.
     */
    void sendMessage(char receiverId, String messagetext);

private:
     /**
     * @brief Adds an acknowledgment for the given message to the send stack.
     * 
     * Constructs an acknowledgment message based on the provided message content and adds it to the send stack. 
     * The acknowledgment message is addressed to the sender of the received message.
     * 
     * @param message The message for which to send an acknowledgment.
     */
    void sendAck(Message* message);

    /**
     * @brief Prints the content of the received message.
     * 
     * Outputs the string representation of the message to the serial monitor.
     * 
     * @param message The message to be printed.
     */
    void printMessage(Message* message);
};

#endif // MESSAGE_SERVICE_H