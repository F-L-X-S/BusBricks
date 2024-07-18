#ifndef INTERFACE_H
#define INTERFACE_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    using namespace std;
#endif

#include <CommInterface.h>
#include <Service.h>
#include <ServiceCluster.h>

#define MAXFRAMESIZE 256 //Max number of Bytes a frame can contain

// Template for generic Interface
// The derived class defines the handling of incoming frames from communication-interface to the services (associated by service-id)
// and the conversion from PDU provided by Services to frames getting sent by the communication-interface
template<typename commInterface_type>                                       
class Interface{
    private:
        CommInterface<commInterface_type> comm_interface;               // Communication-Interface 
        ServiceClusterBase services;                                    // ServiceCluster containing the Services associated with the Interface 
        bool send(){

        };
        
    protected:
        virtual bool getPDU_from_services()=0;
        virtual bool addPDU_to_service()=0;
        virtual bool pdu_to_frame()=0;
        virtual bool frame_to_pdu()=0;
    public:
        Interface(comm_interface_type communication_interface, ServiceCluster* services):comm_interface(communication_interface), services(services) {} 
};
#endif // INTERFACE_H