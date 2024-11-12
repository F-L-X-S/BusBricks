/**
 * @file Component.h
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


#ifndef COMPONENT_H
#define COMPONENT_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <ErrorState.h>

class Component: public ErrorState{
    public:
        /**
         * @brief Default-Constructor for a new Component object
         * 
         */
        Component();

        /**
         * @brief Construct a new Component object
         * 
         * @param componentId Id of the Component-type (e.g. Service-Id or CommInterface-Id)
         * @param instanceId Id of the Component-Instance to identify and address an instance in the network
         */
        Component(uint8_t _componentId, uint8_t _instanceId):
            ErrorState(),
            componentId(_componentId),
            instanceId(_instanceId)
            {};  

        /**
         * @brief Get pointer to the 1-byte Component-ID (equal for connected Components all over the network)
         * 
         * @return uint8_t* Pointer to Component-ID
         */
        uint8_t* getComponentId(){
            return &componentId;
        };

        /**
         * @brief Get pointer to the 1-byte InstanceID of the Service-instance (unique for each Instance of Service within the whole communication-network)
         * 
         * @return uint8_t* InstanceID of the Service-instance
         */
        uint8_t* getInstanceId(){
            return &instanceId;
        };

    protected:
        /**
         * @brief Id of the Component-type (e.g. Service-Id or CommInterface-Id)
         * Has to be equal for connected Components all over the network (e.g. 'm' for all Message-Services)
         */
        uint8_t componentId;

        /**
         * @brief Id of the Component-Instance to identify and address an instance in the network. 
         * Has to be unique for this Component-type all over the network
         */
        uint8_t instanceId;
};


#endif // COMPONENT_H