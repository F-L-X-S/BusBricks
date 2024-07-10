#ifndef CONTENT_H
#define CONTENT_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    #include <cstring>
    using namespace std;
#endif

// Template for defining a Class for mapping a generic (byte-formatted) representation to a processable data-structure
template<typename content_type, typename representation_type>
class Content {
public: 
    // Construct Content-class from an instance from unspecific type
    Content(content_type content) : content(content) {}         

    // Construct Content-class from byte-representation
    Content(representation_type representation) : representation(representation) {}    

    // Destroy Content-element
    virtual ~Content() {
    }
    
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
    virtual void rep_to_content() = 0;

    // Convert the content-attribute to a representation-attribute, has to be defined in derived class (=0)
    virtual void content_to_rep() = 0;  
};

#endif // CONTENT_H