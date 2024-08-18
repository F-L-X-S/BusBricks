/**
 * @file Frame_modbusRTU.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef FRAME_MODBUSRTU_H
#define FRAME_MODBUSRTU_H
#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include<Frame.h>

/// @brief two bytes prefix in frame before pdu (Device-adress, modbus-functioncode)
#define PREFIXSIZE 2    

/// @brief two bytes suffix in frame after pdu (CRC HighByte / LowByte)
#define SUFFIXSIZE 2    

/// @brief CRC-16 mask to use (modbus-rtu default 0xA001)
#define CRC16MASK 0xA001

/// @brief CRC-16 startvalue to use (modbus-rtu default 0xFFFF)
#define CRC16VALUE 0xFFFF



/**
 * @brief Class for defining the structure of the Modbus-RTU-Frame. Can be constructed by a given modbus-frame and extracts payload, 
 * device-id and function-code (additionally checking CRC) or by providing payload, device-id and function-code (additionally calculating CRC).
 * 
 * Because the frame-representation is stored as a reference, the representation has to be allocated on the heap-memory (buffer)
 * The allocation is done during the conversion from "Content" (construction by PDU) within the content_to_rep-function
 * or it has to be initiated by calling the function "copy_to_heap" (e.g. in copy-constructor or during construction by frame-representation)
 * 
 */
class Frame_modbusRTU: public Frame{
    public:
        // Construct Modbus-RRU-Frame from given PDU (Content)
        /**
         * @brief Construct a new Frame_modbusRTU object by PDU, modbus-slave-ID and modus-function-code. 
         * Object calculates and adds the CRC16, the Modbus-Slave-ID and the Modbus-Function-Code and
         * concatenates them to the modbus-frame (representation) by calling content_to_rep during construction. 
         * Allocate heap-memory (buffer) to the frame-representation and let the Content's representation (parent-class-template)
         * point to that buffer on heap.
         * 
         * @param pdu 
         * @param slaveId 
         * @param functionCode 
         */
        Frame_modbusRTU(pduString* pdu, char* slaveId, char* functionCode);

        /**
         * @brief Construct Modbus-RTU-Frame from given Byte-Frame (Representation). Copy the representation, the argument is pointing to,
         * to heap-memory and let by calling copy_to_heap and let Content's representation (parent-class-template) point to that buffer on heap.
         * Extract Payload, device-ID and function-Code. Check the CRC16 for plausibility by checking rest = 0. 
         * 
         * @param frame const char** to a modbus-rtu-frame to construct the Frame-object from
         */
        Frame_modbusRTU(frameString* frame);

        /**
         * @brief Construct empty Modbus-RTU-Frame 
         * 
         */
        Frame_modbusRTU();

        /**
         * @brief Destroy the Frame_modbusRTU object
         * 
         */
        ~Frame_modbusRTU();

        /**
         * @brief Copy constructor.
         *
         * Initializes a new Frame_modbusRTU object as a copy of another Frame_modbusRTU object.
         * 
         * @param other The other Frame_modbusRTU object to copy.
         * 
         * @details This constructor performs a deep copy of all the data from `other`, 
         * ensuring that the new object is an exact copy of the source object.
         */
        Frame_modbusRTU(const Frame_modbusRTU& other); 

        /**
         * @brief Assignment operator.
         *
         * This operator assigns the values from another object of the same type to the current object.
         * 
         * @param other The other Frame_modbusRTU object whose values will be assigned.
         * @return A reference to the current object after assignment.
         * 
         * @details This operator performs a deep copy of the data from `other`, ensuring that the current object 
         * becomes an exact copy of `other`.
         */
        Frame_modbusRTU& operator=(const Frame_modbusRTU& other);

        /**
         * @brief Verifies the CRC16 checksum of the current Modbus RTU frame.
         *
         * This function calculates the CRC16 checksum of the frame's representation and checks 
         * whether it is correct.
         * 
         * @return true if the CRC16 checksum is correct, `false` otherwise.
         * 
         * @details The function creates a temporary buffer, copies the frame's representation to it, 
         * and then compares the calculated CRC16 checksum with the expected value. 
         * The temporary buffer is deleted after use.
         */
        bool checkCRC16();

        /**
         * @brief Get the Modbus-RTU-function-code of the frame-instance 
         * 
         * @return char Modbus-RTU-function-code of the frame-instance 
         */
        char getFunctionCode();

    private:
        /// @brief  Modbus-RTU-slave-id (device-id)
        char slaveId = '\0';

        /// @brief Modbus-RTU-function-code
        char functionCode = '\0';

        /// @brief buffer for allocating heap-memory of dynamic length to the frames-representation 
        char* buffer = new char[1];

        /**
         * @brief Converts the given Content (PDU) to the Modbus RTU frame representation.
         *
         * This function creates a buffer containing the full Modbus RTU frame, 
         * which includes the slave address, function code, content (PDU), and the calculated CRC16 checksum.
         * The resulting frame is stored in the `representation` member.
         * 
         * @details The buffer is dynamically allocated based on the size of the content (PDU) plus space for 
         * the prefix (slave address and function code) and suffix (CRC16). The buffer is null-terminated 
         * and assigned to the `representation`. The function also handles memory management by deleting 
         * any previously allocated buffer before creating a new one.
         */
        void content_to_rep()  override;

        /**
         * @brief Converts the given frame representation (Modbus RTU frame) to the Content (PDU).
         *
         * This function extracts the content (PDU) from the Modbus RTU frame representation, 
         * along with the slave ID and function code. The content is extracted only if the frame 
         * is long enough to contain a valid PDU.
         * 
         * @details If the frame is shorter than the expected minimum size (prefix + suffix), 
         * the content will remain empty. The slave ID and function code are extracted from the first 
         * two bytes of the frame, and the PDU content is extracted from the bytes in between the prefix 
         * and suffix. 
         * 
         * @note The function assumes that the representation is null-terminated.
         */
        void rep_to_content() override;

        /**
         * @brief Calculates the CRC16 checksum of the given buffer.
         *
         * This function computes the CRC16 checksum for the provided buffer using the standard 
         * Modbus RTU CRC16 algorithm. The calculation is performed byte-by-byte, applying bitwise 
         * operations to generate the checksum.
         * 
         * @param crcBuffer A pointer to the buffer containing the data for which the CRC16 checksum is calculated.
         * @param size The size of the buffer in bytes.
         * @return The calculated CRC16 checksum as an unsigned short.
         * 
         * @details The CRC16 algorithm uses a specific initialization value and applies a polynomial 
         * mask during the calculation. The function iterates over each byte in the buffer, updating the 
         * checksum according to the Modbus RTU CRC16 standard.
         */
        unsigned short calcCRC16(char* buffer, uint8_t size);

        /**
         * @brief Copies the representation pointing to an externally defined char-array to heap memory.
         *
         * This function ensures that the provided string, which may reside in stack memory, 
         * is copied to heap memory. This is necessary to ensure that the destructor correctly 
         * frees memory from the heap and avoids attempting to delete stack-allocated memory.
         * 
         * @param str_ptr A double pointer to the externally defined char-array that will be copied to heap memory.
         * 
         * @details The function allocates a new buffer on the heap, copies the contents of the given 
         * string into this buffer, and ensures null-termination. After copying, it updates the pointer 
         * reference to point to the newly allocated heap memory.
         * 
         * @note The function deletes any previously allocated heap memory pointed to by `buffer` 
         * before allocating the new memory.
         */
        void copy_to_heap(const char** str_ptr);

    };


#endif // FRAME_MODBUSRTU_H