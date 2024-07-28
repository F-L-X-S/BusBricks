#ifndef CONTENT_H
#define CONTENT_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
    #include <cstring>
#endif

// Template for defining a Class for mapping a generic (byte-formatted) representation to a processable data-structure
// class-attributes are a copy of the arguments passed to the constructor (call-by-value)
template<typename content_type, typename representation_type>
class Content {
public: 
    // Construct Content-class from an instance from unspecific type
    Content(content_type content) : content(content) {}         

    // Construct Content-class from byte-representation
    Content(representation_type representation) : representation(representation) {}    
    
    // Get the representation as a the representation-type
    representation_type* get_representation() {
        return &representation;
    }                               

    // Get the Content formatted as content_type from the Object 
    content_type* get_content() {                                          
        return &content;
    }

protected:
    content_type content;                   // Content
    representation_type representation;     // Representation

    // Convert the representation-attribute to the content-attribute, has to be defined in derived class (=0)
    // the function has to be called in the constructor of the derived class 
    virtual void rep_to_content() = 0;

    // Convert the content-attribute to a representation-attribute, has to be defined in derived class (=0)
    // the function has to be called in the constructor of the derived class 
    virtual void content_to_rep() = 0;  
};

#endif // CONTENT_H