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

// Structure to define byte-representation and size 
struct byte_representation{
    char* representation_arr[0];             // ptr to start-char of representation
    uint8_t* size;                       // size of representation
        
    ~byte_representation() {
        delete[] representation_arr;
        delete size;
    }
};

// Template for defining a Class for mapping a generic byte-formatted representation to a processable data-structure
template<typename content_type>
class Content{
public: 
    // Construct Content-class from an instance from unspecific type
    Content(content_type* content) : content(content) {
        allocate_representation_mem();      // allocate memory to the representation-attribute
    }         

    // Construct Content-class from byte-representation
    Content(byte_representation* representation) : representation(representation) {
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
    
    // Get the representation as a byte_representation-structure
    byte_representation* get_representation(){
        return representation;
    };                               

    // Get the Content formatted as content_type from the Object 
    content_type* get_content(){                                          
        return content;
    };

protected:
    content_type* content=nullptr;                                        // Content, dynamically  allocated if not already instanciated
    byte_representation* representation=nullptr;                          // Representation, dynamically  allocated if not already instanciated

    // Convert the byte-formatted representation to a content-object, has to be defined in derived class (=0)
    virtual content_type* rep_to_content(byte_representation* representation)=0;

    // Convert the content-object to a byte-formatted representation, has to be defined in derived class (=0)
    virtual byte_representation* content_to_rep(content_type* content)=0;  

private:

    // Allocate Memory to the content-object 
    void allocate_content_mem(){
        if (content==nullptr)
        {
            content = new content_type;
            content = *rep_to_content(representation);
        }
    }

    // Allocate Memory to the representation-structure
    void allocate_representation_mem(){
        if (representation==nullptr)
        {
            representation = new byte_representation;
            representation = *content_to_rep(content);
        }
    }
};

#endif // CONTENT_H