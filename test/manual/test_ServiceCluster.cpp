#include "ServiceCluster.h"
#include "Message.h"

#define SERVICEID_1 0x41    // ASCII "a"
#define SERVICEID_2 0x42    // ASCII "b"
#define SERVICEID_3 0x43    // ASCII "c"

#define STACKSIZE 3

int main(){
    Service<Message, STACKSIZE> service_1(SERVICEID_1);       // Service 1 as general Message-Service
    Service<Message, STACKSIZE> service_2(SERVICEID_2);       // Service 2 as general 32-bit-Int-Service
    Service<Message, STACKSIZE> service_3(SERVICEID_3);       // Service 3 as general char-Service

    ServiceBase* serviceList[3] = {&service_1, &service_2, &service_3};     // Array of ptr to the services 

    ServiceCluster<3> services(serviceList);                                // Create a Service-Cluster from ptr-list to the associated services 

    ServiceBase* ret_service_1 = services.getService_byID(SERVICEID_1);
    std::cout<<"\nService-ID of Service "<<static_cast<char>(SERVICEID_1)<<":\t"<<static_cast<char>(*ret_service_1->get_ServiceID())<<std::endl;

    ServiceBase* ret_service_2 = services.getService_byID(SERVICEID_2);
    std::cout<<"\nService-ID of Service "<<static_cast<char>(SERVICEID_2)<<":\t"<<static_cast<char>(*ret_service_2->get_ServiceID())<<std::endl;

    ServiceBase* ret_service_3 = services.getService_byID(SERVICEID_3);
    std::cout<<"\nService-ID of Service "<<static_cast<char>(SERVICEID_3)<<":\t"<<static_cast<char>(*ret_service_3->get_ServiceID())<<std::endl;
}