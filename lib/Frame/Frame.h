/**
 * @file Frame.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef FRAME_H
#define FRAME_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include<Content.h>

/** Typedefinitions for Constructor-parameters of the Frame-class 
 * necessary to distinguish between construction from PDU (Content) or from byte-frame (Representation) 
*/
using pduString = String;
using frameString = const char*;

/**
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief Frame-Class as derived class from Content
The derived classes define:
-the conversion from a given payload plus protocol-specific parameters (e.g. Modbus-function-code, addresses...)
 to the frame-representation (override of content_to_rep from Content-Class)
- the extraction of protocol-specific parameters from the byte-representation (frame) 
 with eventually checking their validity (e.g. CRC) (override of rep_to_content from Content-Class
In the abstract concept of "Content" the byte-Frame itself is interpreted as the Content-Representation
The "Content"-Object represents the PDU transmitted to the services 
 *  
 * @tparam pduString type-definition for a String-Object containing the Payload of the frame ("Content")
 * @tparam frameString type-definition for a const-char-pointer pointing to the byte-formatted frame 
 */
class Frame: public Content<pduString, frameString>{
    public:
       /**
        * @brief Construct a new empty Frame object
        * 
        */
        Frame();

       /**
        * @brief Destroy the Frame object
        * 
        */
        ~Frame();

       /**
        * @brief Constructor for creating Frame-instance from pdu (Content)
        * 
        * @param pdu pointer to pduString containing the payload of the frame
        */
        Frame(pduString* pdu);

       /**
        * @brief Constructor for creating Frame-Object from byte-frame (Representation)
        * 
        * @param frame pointer to frameString containing the byte-frame (Representation) of the frame
        */
        Frame(frameString* frame);

       /**
        * @brief get the pdu (Content) as String from the frame-object
        * 
        * @return String with frame-payload
        */
        String getPDU();

       /**
        * @brief Get the byte-frame (Representation) as String from the frame-object 
        * 
        * @return String with byte-frame (Representation)
        */
        String getFrame();
};
#endif // FRAME_H