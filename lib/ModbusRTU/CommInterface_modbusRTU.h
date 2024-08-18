/**
 * @file CommInterface_modbusRTU.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef COMMINTERFACE_MODBUSRTU
#define COMMINTERFACE_MODBUSRTU
#ifdef ARDUINO
    #include <Arduino.h>
    #include <SoftwareSerial.h> 
    typedef SoftwareSerial SoftwareSerial;  // typedef for clang-build
#else   
    #include <mockSoftwareSerial.h>         // include Library to support testing with SoftwareSerial in native-environment 
    #include <mockArduino.h>                // include Library to support testing with Arduino-specific functions in native-environment 
    typedef arduinoMocking::SoftwareSerial SoftwareSerial; // typedef for clang-build
    using namespace arduinoMocking;
#endif

#include <CommInterface.h>

/// @brief debugging flag to prit debugging-information on Serial 
#define DEBUG

/// @brief maximum number of Bytes a frame can contain
#define MAXFRAMESIZE 256 

/**
 * @brief The Modbus-RTU CommInterface-class specifies a communication interface with SoftwareSerial as interface-type
 * and Monitors modbus-specific timeouts. Ensures correct data-transfer between standardized Frame-Objects in the Interface-Stack and SoftwareSerial.
 * 
 */
class CommInterface_modbusRTU: public CommInterface<SoftwareSerial>{
    public:
        /**
         * @brief Construct a new CommInterface_modbusRTU object 
         * 
         * @param softwareserial pointer to the SoftwareSerial-instance, the Comminterface should use  
         * @param baudrate baudrate in bps to calculate timeouts 
         * @param deviceId Modbus-specific device-identifier 
         */
        CommInterface_modbusRTU(SoftwareSerial* softwareserial ,uint16_t baudrate, char deviceId);

        /**
         * @brief Destroy the CommInterface_modbusRTU object
         * 
         */
        ~CommInterface_modbusRTU();

        /**
         * @brief Override of CommInterface send-function. 
         * Send the frame, sendBuffer is pointing to by writing and flushing SoftwareSerial-interface
         * 
         * @return true frame was flushed to serial-bus
         * @return false sendBuffer is nullptr
         */
        bool send() override;

        /**
         * @brief Receives a Modbus RTU frame and writes it to `receiveBuffer`.
         *
         * Waits for incoming bytes for a duration defined by `_recTimeout`. If no bytes are received within this period, the function exits. 
         * When bytes are received via SoftwareSerial, the function starts writing the frame to `receiveBuffer` once a byte containing the 
         * device ID of the `CommInterface` instance is detected. If the device ID is the null terminator (master mode), all incoming frames are captured.
         * Bytes related to the frame must be received within the `_charTimeout`.
         * 
         * The function continues receiving until either `MAXFRAMESIZE` is reached or `_charTimeout` expires. After receiving, it waits for `_frameTimeout` 
         * before clearing the SoftwareSerial receive buffer by calling `_clearRxBuffer`.
         * 
         * @return `true` if a frame was successfully received and written to `receiveBuffer` within the receive timeout.
         * @return `false` if no frame was received during `_recTimeout`.
         * 
         * @details The function operates in two modes:
         * - **Slave Mode**: Begins receiving only if the frame is addressed to the device's ID.
         * - **Master Mode**: Captures any incoming frame.
         * 
         * The function does not validate the content of the received frame. It uses timeouts to ensure the entire frame is received and then clears the communication interface's buffer.
         * 
         * @note If `DEBUG` is defined, debugging information is printed to the serial monitor.
         */
        bool receive() override;

        /**
         * @brief Get Modbus-specific device-identifier
         * 
         * @return char Modbus-specific device-identifier
         */
        char getDeviceId();

    private:
        /// @brief Modbus-RTU Slave ID, default nullterminator for mastermode
        const char deviceId;       

        /// @brief Timeout between two chars received [microseconds]                   
        unsigned long _charTimeout;                     

        /// @brief  minimum time to wait between two Frames [microseconds]      
        unsigned long _frameTimeout;                    

        /// @brief time to wait for response during the receive-cycle [microseconds]      
        unsigned long _recTimeout = 1000000;            

        /**
         * @brief Calculate all timeouts based on the specified baudrate
         * 
         * @param baud bus-baudrate [bps]
         */
        void _calculateTimeouts(uint16_t baud);         

        /**
         * @brief Clear the receive-buffer of SoftwareSerial 
         * 
         */
        void _clearRxBuffer();                          

};

#endif // COMMINTERFACE_MODBUSRTU