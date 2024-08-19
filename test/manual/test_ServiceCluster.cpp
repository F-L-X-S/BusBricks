/**
 * @file test_ServiceCluster.cpp
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


#include "ServiceCluster.h"
#include "Message.h"

#define SERVICEID_1 0x41    // ASCII "a"
#define SERVICEID_2 0x42    // ASCII "b"
#define SERVICEID_3 0x43    // ASCII "c"

#define INSTANCEID_1 0x61   // ASCII "A"
#define INSTANCEID_2 0x62   // ASCII "B"
#define INSTANCEID_3 0x63   // ASCII "C"

#define STACKSIZE 3

int main(){
    Service<Message, STACKSIZE> service_1(SERVICEID_1, INSTANCEID_1);       // Service 1 as general Message-Service
    Service<Message, STACKSIZE> service_2(SERVICEID_2, INSTANCEID_2);       // Service 2 as general 32-bit-Int-Service
    Service<Message, STACKSIZE> service_3(SERVICEID_3, INSTANCEID_3);       // Service 3 as general char-Service

    ServiceBase* serviceList[3] = {&service_1, &service_2, &service_3};     // Array of ptr to the services 

    ServiceCluster<3> services(serviceList);                                // Create a Service-Cluster from ptr-list to the associated services 

    ServiceBase* ret_service_1 = services.getService_byID(SERVICEID_1);
    std::cout<<"\nInstance-ID of Service "<<static_cast<char>(SERVICEID_1)<<":\t"<<static_cast<char>(*ret_service_1->get_InstanceID())<<std::endl;

    ServiceBase* ret_service_2 = services.getService_byID(SERVICEID_2);
    std::cout<<"\nInstance-ID of Service "<<static_cast<char>(SERVICEID_2)<<":\t"<<static_cast<char>(*ret_service_2->get_InstanceID())<<std::endl;

    ServiceBase* ret_service_3 = services.getService_byID(SERVICEID_3);
    std::cout<<"\nInstance-ID of Service "<<static_cast<char>(SERVICEID_3)<<":\t"<<static_cast<char>(*ret_service_3->get_InstanceID())<<std::endl;
}