/**
 * @file SocketRouter.h
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

#ifndef SOCKETROUTER_H
#define SOCKETROUTER_H
#ifdef ARDUINO
    #include <Arduino.h>    // include Arduino-Library for platformIO-build 
#else
    #include <mockArduino.h>
    using namespace arduinoMocking;
#endif

#include <Socket.h>
#include <Cluster.h>

class SocketRouter{
    public:

    private:
        /// @brief pointer to ServiceCluster, containing the Services associated with the Interface (instance of ServiceCluster-derived class) 
        ClusterBase* serviceCluster; 

        /// @brief pointer to CommInterface-Cluster, containing the Communication-Interfaces associated with the Router 
        ClusterBase* commCluster;
};

#endif //SOCKETROUTER_H