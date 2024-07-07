#ifndef SERVICECLUSTER_H
#define SERVICECLUSTER_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
    #include "Arduino_std.h"// import std-namesace for Arduino-build
    using namespace std;    // use std-namespace from Arduino_std
#else
    #include <iostream>     // include iostream for local testing 
    using namespace std;
#endif

#include <Service.h>

class ServiceClusterBase{
    public:
        virtual ServiceBase* getService_byID(uint8_t ServiceID)=0;
        virtual ~ServiceClusterBase(){};
};

// The ServiceCluster provides functions to manage multiple services 
template<uint8_t number_of_services>         
class ServiceCluster: public ServiceClusterBase{
    private:
        ServiceBase* services[number_of_services];
    public:
        // Constructor
        ServiceCluster(ServiceBase* serviceList[number_of_services]) {
                    for (int i = 0; i < number_of_services; ++i) {
                        services[i] = serviceList[i];
        }
        }

        // Get a Pointer to the Service with the given ID 
        ServiceBase* getService_byID(uint8_t ServiceID) override {
            for (int i = 0; i < number_of_services; ++i) {
                if (services[i] && *services[i]->get_ServiceID() == ServiceID) {
                    return services[i];
                }
            }
            return nullptr;     // no Service with given ID 
        }
};

#endif // SERVICECLUSTER_H