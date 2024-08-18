/**
 * @file Service.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef SERVICE_H
#define SERVICE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
    #include <cstring>
#endif

#include "Content.h"
#include "Content_stack.h"

/**
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief Service-base-class to add class-functions to vtable
 * 
 */
class ServiceBase {
public:
    /**
     * @brief Get the get the response-payload, stored at the response_pdu of the service instance  
     * 
     * @return String response-pdu as String
     */
    virtual String get_response()=0;

    /**
     * @brief Check, if a response of the service is available (services sendStack not empty)
     * 
     * @return true a response-payload is available 
     * @return false no response-payload is available
     */
    virtual bool responseAvailable()=0;

    /**
     * @brief Clear the response-buffer (delete item, that was returned by get_response from the service-sendStack)
     * 
     */
    virtual void clearResponse()=0;

    /**
     * @brief Execute the service's functions for each item on the receive-stack and add all response-payloads to be send to the send-stack
     */
    virtual void stackProcessing()=0;

    /**
     * @brief Add a new Content-Object created from a received payload to the services receive-Stack. 
     * The payload is the representation of the Content-Object, during the construction, the Content-object creates it's content from the representation.
     * The Content-object is added to the rec_stack.
     * 
     * @param pdu payload from received Frame-object as pointer to string
     * @return true content-object added to service-rec_stack
     * @return false rec_stack is full, content-object not added 
     */
    virtual bool impart_pdu(String* pdu)=0;

    /**
     * @brief Get pointer to the 1-byte ServiceID of the Service (unique for each Service-template derived Class)
     * 
     * @return uint8_t* ServiceID of the service
     */
    virtual uint8_t* get_ServiceID()=0;

    /**
     * @brief Get pointer to the 1-byte InstanceID of the Service-instance (unique for each Instance of Service within the whole communication-network)
     * 
     * @return uint8_t* InstanceID of the Service-instance
     */
    virtual uint8_t* get_InstanceID()=0;

    /**
     * @brief Destroy the Service Base object
     * 
     */
    virtual ~ServiceBase(){};
};

/**
 * 
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief Service-Template to derive a Service class by defining the Content (derived Class of "Content") to handle and the size of the stacks (send and receive).
 * A Service has to be instantiated with a unique service-ID and an instance-ID, the service-instance should use for it's communication.
 * E.g.: Messenger-service with service-id "m" (specified in derived class, same on every host) is instantiated with a host-specific ID to identify the instances.
 * The derived classes have to define the stack-processing to handle the payload at the rec-stack and add payload to the send-stack. Each derived class has a unique serviceID
 * to identify the Service-type and each instance in the communication-network has a unique instanceID. 
 * 
 * @tparam content_class type of the items stored in the stack 
 * @tparam stackSize type of the items stored in the stack 
 */
template<typename content_class, int stackSize>                                       
class Service: public ServiceBase{
protected: 
    uint8_t serviceID;                                                                                // service-id
    uint8_t instanceID;                                                                               // service-instance-id                                
    Content_stack<content_class, stackSize> rec_stack;                                                // stack for received content-elements
    Content_stack<content_class, stackSize> send_stack;                                               // stack for received content-elements
    String response_pdu;                                                                              // PDU with response 
    
    // write the response-PDU to the reserved memory of the service-instance 
    void write_response_pdu(content_class response_object){
        response_pdu = *response_object.get_representation();
    }

public: 
    Service(uint8_t serviceID, uint8_t instanceID): serviceID(serviceID), instanceID(instanceID){}  

    /**
     * @brief Get pointer to the 1-byte ServiceID of the Service (unique for each Service-template derived Class)
     * 
     * @return uint8_t* ServiceID of the service
     */
    uint8_t* get_ServiceID() override {
        return &serviceID;
    }

    /**
     * @brief Get pointer to the 1-byte InstanceID of the Service-instance (unique for each Instance of Service within the whole communication-network)
     * 
     * @return uint8_t* InstanceID of the Service-instance
     */
    uint8_t* get_InstanceID() override {
        return &instanceID;
    }

    /**
     * @brief Add a new Content-Object created from a received payload to the services receive-Stack. 
     * The payload is the representation of the Content-Object, during the construction, the Content-object creates it's content from the representation.
     * The Content-object is added to the rec_stack.
     * 
     * @param pdu payload from received Frame-object as pointer to string
     * @return true content-object added to service-rec_stack
     * @return false rec_stack is full, content-object not added 
     */
    virtual bool impart_pdu(String* pdu) override {                                                     
        content_class content(pdu);                                                          // create Content-object from PDU
        return rec_stack.addElement(content);                                                // add Content-Object to Rec-Stack
    };

    /**
     * @brief Get the get the response-payload, stored at the response_pdu of the service instance  
     * 
     * @return String response-pdu as String
     */
    virtual String get_response() override {                                                                     
        // copy the response element from send-stack
        content_class response_element = (send_stack.empty())? content_class():*send_stack.getElement();
        // write pdu from this element to response-pdu-memory of service-instance 
        write_response_pdu(response_element);
        // return the response-pdu of the service-instance
        return response_pdu;
    };             

    /**
     * @brief Check, if a response of the service is available (services sendStack not empty)
     * 
     * @return true a response-payload is available 
     * @return false no response-payload is available
     */
    virtual bool responseAvailable() override {                                                                      
        return !send_stack.empty();
    };            

    /**
     * @brief Clear the response-buffer (delete item, that was returned by get_response from the service-sendStack)
     * 
     */
    virtual void clearResponse() override {   
        send_stack.deleteElement();             // delete last element from sendstack
        response_pdu = "";
    };                                                 

    /**
     * @brief Execute the service's functions for each item on the receive-stack and add all response-payloads to be send to the send-stack 
     */
    virtual void stackProcessing() override {
        if (rec_stack.empty()) return;
        while(!rec_stack.empty()){
            content_class recItem = *rec_stack.getElement(); // example implementation: adding received items back to send-stack
            send_stack.addElement(recItem);
            rec_stack.deleteElement();
        }        
    }                   
};

#endif // SERVICE_H