#include <Message_service.h>
#include <Frame_modbusRTU.h>
#include <CommInterface_modbusRTU.h>

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <mockSoftwareSerial.h>     // include Library to support testing with SoftwareSerial in native-environment 
    #include <mockArduino.h>            // include Library to support testing with Arduino-specific functions in native-environment 
    using namespace arduinoMocking;
#endif

void setup() {
    Serial.println("Setup...");
};

void loop() {
    Serial.print("loop...");
 };
