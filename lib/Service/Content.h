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

// Template for defining a Class for mapping a generic byte-formatted representation to a processable data-structure
template<typename content_type>
class Content{
public: 
    // Construct Content-class from an instance from unspecific type
    Content(content_type* content) : content(content) {
        allocate_representation_mem();      // allocate memory to the representation-attribute
    }         

    // Construct Content-class from byte-representation
    Content(char* byte_representation, uint8_t* rep_size) : representation(byte_representation, rep_size) {
        allocate_content_mem();             // allocate memory to the content-attribute 
    }    

    // Destroy Content-element
    // delete all dynamically allocated memories
    virtual ~Content() {
        if (representation!=nullptr)
        {
            delete[] representation;
        }
        if (content!=nullptr)
        {
            delete[] content;
        }
    }
    
    // Get the representation as a char-array 
    char* get_representation(){
        return *representation;
    };                               

    // Get the Content formatted as content_type from the Object 
    content_type* get_content(){                                          
        return *content;
    };

protected:
    // Convert the byte-formatted representation to a content-object, has to be defined in derived class (=0)
    virtual content_type* rep_to_content(char* byte_rep, uint8_t* rep_size)=0;

    // Convert the content-object to a byte-formatted representation , has to be defined in derived class (=0)
    virtual char* content_to_rep(content_type* content)=0;  

private:
    content_type* content=nullptr;                                        // Content, dynamically  allocated if not already instanciated
    char* representation=nullptr;                                         // Representation, dynamically  allocated if not already instanciated
    uint8_t* rep_size=nullptr;                                            // size (number of bytes) of the representation

    // Allocate Memory to the content-object 
    void allocate_content_mem(){
        if (content==nullptr)
        {
            content = new content_type;
            content = rep_to_content(representation, rep_size);
        }
    }

    // Allocate Memory to the representation and the size
    void allocate_representation_mem(){
        if (representation==nullptr)
        {
            representation = new char;
            representation = content_to_rep(content);
        }
    }
};

#endif // CONTENT_H