#ifndef ARDUINO_STD
    #define ARDUINO_STD
    #ifdef ARDUINO
        #include <Arduino.h> 

        namespace std {
        // Typedefinition for std::string
        typedef String string;
 
        // replace std::cout
        template<typename T>
        void cout(T arg) {
            Serial.print(arg);
        }

        // replace std::endl
        void endl() {
            Serial.println();
        }
    }
    #endif
#endif
