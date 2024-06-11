#ifndef DEVICE_H
    #define DEVICE_H

    #include "Interface.h"
    #define CRC16MASK 0xA001
    #define CRC16VALUE 0xFFFF
    #define MAXFRAMESIZE 256 //uint in Bytes

    class Device
    {
        private:
            char buffer[256]; //Buffer for complete frame
            short calc_crc(const unsigned char* buffer); //calculate the CRC16 for the stored frame
            bool check_crc(const unsigned char* buffer); //check the CRC16 for the stored frame
            virtual char construct_frame()=0; //construct the byte-formatted frame from payload, function has to be provided in derived class
            virtual void decode_frame()=0; //decode the byte-formatted frame to payload, function has to be provided in derived class

        protected:
            Device(Interface& interface); // Construct the Modbus-device-instance with the belonging hardware-interface, abstract class, instanciation only for derived classes
    };
#endif