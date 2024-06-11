#ifndef CONTENT_H
#define CONTENT_H

template<typename content_type>

class Content
{
    public: 
        Content(content_type* data) : content(data) {}    // Construct Content-class with an unspecific type

    protected:
        content_type* content;                 // Pointer to unspecified type, depends on type of payload 
        virtual char* content_to_pdu()=0;           // has to be defined in derived class
        virtual content_type* pdu_to_content()=0;   // has to be defined in derived class
};

#endif