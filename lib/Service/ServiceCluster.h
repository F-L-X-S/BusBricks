#ifndef SERVICECLUSTER_H
#define SERVICECLUSTER_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <Service.h>

class ServiceClusterBase{
    public:
        virtual ServiceBase* getService_byID(uint8_t ServiceID)=0;
        virtual ServiceBase* getService_byPos(uint8_t ServicePosition)=0;
        virtual uint8_t getNumberOfServices() const =0;
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

        // Get a Pointer to the Service at the given position in cluster (0-based)
        ServiceBase* getService_byPos(uint8_t ServicePosition) override {
            if (services[ServicePosition]) {
                return services[ServicePosition];
            }
            return nullptr;     // no Service with given ID 
        }

        // Function to return the number of services
        uint8_t getNumberOfServices() const override {
            return number_of_services;
        }
};

#endif // SERVICECLUSTER_H