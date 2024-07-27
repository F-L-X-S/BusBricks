#ifndef FUNCTION_H
#define FUNCTION_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
    #include <cstring>
#endif

#include "Content.h"

// Template to create a Function class by defining the content-type to handle 
template<typename content_type>                                       
class Function{
public: 
    Function() : {}                                                                         // Construct Function-class with an unspecific type
    virtual bool exec_condition(content_type* element)=0;                                   // check, if function should be executed for given Content-instance 
    virtual bool exec_action(content_type* element)=0;                                       
};

#endif // FUNCTION_H