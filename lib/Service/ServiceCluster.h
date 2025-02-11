/**
 * @file ServiceCluster.h
 * @author Felix Schuelke (flxscode@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-18
 * 
 * @copyright * Copyright (C) 2024 Felix Schuelke
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
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
#include <ErrorState.h>
#include <Frame.h>

/**
 * @brief ServiceCluster-base-class to add class-functions to vtable
 * 
 */
class ServiceClusterBase: public ErrorState{
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
         * @brief Add the payload of the referenced Frame to the belonging Service 
         * 
         * @param FrameToAdd Frame, thats PDU has to be added to the Service 
         * @return true PDU added successfully
         * @return false failed to add PDU
         */
        virtual errorCodes impartPdu(Frame* FrameToAdd)=0;

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

        /**
         * @brief Add the payload of the referenced Frame to the belonging Service (Multiplexing by Service-ID)
         * 
         * @param FrameToAdd Frame, thats PDU has to be added to the Service 
         * @return true PDU added successfully
         * @return false failed to add PDU
         */
        errorCodes impartPdu(Frame* FrameToAdd) override {
            char ServiceID = FrameToAdd->getServiceId();                                // Get the Service-ID of the given Frame
            ServiceBase* destinationService = getService_byID(ServiceID);     // Pointer to the destination-Service 
            if (!destinationService){
                raiseError(serviceNotFound);                                            // raise Service-not-found-error if no Service with this Service-ID exists
                return getErrorState();
            }
            String pdu = *FrameToAdd->get_content();                                    // Get the Frames payload 
            if (!destinationService->impart_pdu(&pdu)){                                 // Add a Content-Object created from PDU to the Services receive-stack 
                raiseError(overflow);                                       // raise Overflow-Error, if the Rec-Stack of the Service is full
            };                                       
            return getErrorState();
        };
};

#endif // SERVICECLUSTER_H