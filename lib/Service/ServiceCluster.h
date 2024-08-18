/**
 * @file ServiceCluster.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SERVICECLUSTER_H
#define SERVICECLUSTER_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <Service.h>

/**
 * @brief ServiceCluster-base-class to add class-functions to vtable
 * 
 */
class ServiceClusterBase{
    public:
        /**
         * @brief Get pointer to the service with the given service-ID by iterating through the associated services and calling their get_ServiceID-function.
         * 
         * @param ServiceID 1-byte service-ID (unique for service-class)
         * @return ServiceBase* pointer to the service with the given ID 
         */
        virtual ServiceBase* getService_byID(uint8_t ServiceID)=0;

        /**
         * @brief Get pointer to the service at a specific position in the cluster (positions starting by zero)
         * 
         * @param ServicePosition position of the service in the cluster (0-based)
         * @return ServiceBase* pointer to the service at the given position
         */
        virtual ServiceBase* getService_byPos(uint8_t ServicePosition)=0;

        /**
         * @brief Get the total Number Of Services-objects associated to the cluster 
         * 
         * @return uint8_t total Number Of Services-objects associated to the cluster 
         */
        virtual uint8_t getNumberOfServices() const =0;

        /**
         * @brief Destroy the Service Cluster Base object
         * 
         */
        virtual ~ServiceClusterBase(){};
};


/**
 * @author Felix Schuelke 
 * @brief The ServiceCluster provides functions to manage multiple services. 
 * The ServiceCluster is added to the ServiceInterface and contains references to all services, associated with the interface.
 * @tparam number_of_services number of service-instances, the cluster contains
 */
template<uint8_t number_of_services>         
class ServiceCluster: public ServiceClusterBase{
    private:
        ServiceBase* services[number_of_services];
    public:
        /**
         * @brief Construct a new Service Cluster object from associated services
         * 
         * @param serviceList Array of references to the associated services
         */
        ServiceCluster(ServiceBase* serviceList[number_of_services]) {
                    for (int i = 0; i < number_of_services; ++i) {
                        services[i] = serviceList[i];
        }
        }

        /**
         * @brief Get pointer to the service with the given service-ID by iterating through the associated services and calling their get_ServiceID-function.
         * 
         * @param ServiceID 1-byte service-ID (unique for service-class)
         * @return ServiceBase* pointer to the service with the given ID 
         */
        ServiceBase* getService_byID(uint8_t ServiceID) override {
            for (int i = 0; i < number_of_services; ++i) {
                if (services[i] && *services[i]->get_ServiceID() == ServiceID) {
                    return services[i];
                }
            }
            return nullptr;     // no Service with given ID 
        }

        /**
         * @brief Get pointer to the service at a specific position in the cluster (positions starting by zero)
         * 
         * @param ServicePosition position of the service in the cluster (0-based)
         * @return ServiceBase* pointer to the service at the given position
         */
        ServiceBase* getService_byPos(uint8_t ServicePosition) override {
            if (services[ServicePosition]) {
                return services[ServicePosition];
            }
            return nullptr;     // no Service with given ID 
        }

        /**
         * @brief Get the total Number Of Services-objects associated to the cluster 
         * 
         * @return uint8_t total Number Of Services-objects associated to the cluster 
         */
        uint8_t getNumberOfServices() const override {
            return number_of_services;
        }
};

#endif // SERVICECLUSTER_H