/**
 * @file Cluster.h
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


#ifndef CLUSTER_H
#define CLUSTER_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include "Component.h"
#include "ErrorState.h"

/**
 * @brief Cluster-base-class to add class-functions to vtable
 * 
 */
class ClusterBase: public virtual ErrorState{
    public:
        /**
         * @brief Get pointer to the Component with the given Component-ID by iterating through the associated Components and calling their getComponentId-function.
         * 
         * @param _componentId 1-byte component-ID 
         * @return Component* pointer to the component with the given ID 
         */
        virtual Component* getComponent_byID(uint8_t _componentId)=0;

        /**
         * @brief Get pointer to the component at a specific position in the cluster (positions starting by zero)
         * 
         * @param _componentPosition position of the component in the cluster (0-based)
         * @return ClusterBase* pointer to the component at the given position
         */
        virtual Component* getComponent_byPos(uint8_t _componentPosition)=0;

        /**
         * @brief Get the total Number Of Component-objects associated to the cluster 
         * 
         * @return uint8_t total Number Of Component-objects associated to the cluster 
         */
        virtual uint8_t getNumberOfComponents() const =0;
};


/**
 * @author Felix Schuelke 
 * @brief The Cluster provides functions to manage multiple Component-derived Objects. 
 * The Cluster contains references to all Components, associated with the interface.
 * @tparam number_of_components number of component-instances, the cluster contains
 */
template<uint8_t number_of_components>         
class Cluster: public ClusterBase{
    private:
        Component* components[number_of_components];
    public:
        /**
         * @brief Construct a new Component Cluster object from associated Components
         * 
         * @param componentsList Array of references to the associated Components
         */
        Cluster(Component* componentsList[number_of_components]):
        ErrorState()
        {
                    for (int i = 0; i < number_of_components; ++i) {
                        components[i] = componentsList[i];
        }
        }

        /**
         * @brief Get pointer to the Component with the given component-ID by iterating through the associated components 
         * and calling their getComponentId-function.
         * 
         * @param ComponentID 1-byte component-ID 
         * @return Component* pointer to the component with the given ID 
         */
        Component* getComponent_byID(uint8_t _componentId) override {
            for (int i = 0; i < number_of_components; ++i) {
                if (components[i] && *components[i]->getComponentId() == _componentId) {
                    return components[i];
                }
            }
            return nullptr;     // no Component with given ID 
        }

        /**
         * @brief Get pointer to the component at a specific position in the cluster (positions starting by zero)
         * 
         * @param _position position of the component in the cluster (0-based)
         * @return Component* pointer to the component at the given position
         */
        Component* getComponent_byPos(uint8_t _position) override {
            if (components[_position]) {
                return components[_position];
            }
            return nullptr;     // no Component with given ID 
        }

        /**
         * @brief Get the total Number Of Component-objects associated to the cluster 
         * 
         * @return uint8_t total Number Of Component-objects associated to the cluster 
         */
        uint8_t getNumberOfComponents() const override {
            return number_of_components;
        }
};

#endif // CLUSTER_H