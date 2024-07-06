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

#include <Comm_interface.h>
#include <Service.h>
#include <ServiceCluster.h>

// Template for generic Interface
// The derived class defines the handling of incoming frames from communication-interface to the services (associated by service-id)
// and the conversion from PDU provided by Services to frames getting sent by the communication-interface
template<typename comm_interface_type>                                       
class Interface{
    private:
        Comm_interface<comm_interface_type> comm_interface;
        ServiceClusterBase services;
    protected:
        virtual bool getPDU_from_services();
        virtual bool addPDU_to_service();
        virtual bool pdu_to_frame();
        virtual bool frame_to_pdu();
    public:
        Interface(comm_interface_type communication_interface, ServiceCluster* services):comm_interface(communication_interface), services(services) {} 
};
#endif // INTERFACE_H