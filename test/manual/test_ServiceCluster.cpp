#include "ServiceCluster.h"
#include "Message.h"

#define SERVICEID_1 0xA1
#define SERVICEID_2 0xB2
#define SERVICEID_3 0xC3

#define INSTANCEID_1 0x1A
#define INSTANCEID_2 0x2B
#define INSTANCEID_3 0x3C

#define STACKSIZE 3

int main(){
    Service<Message, STACKSIZE> service_1(SERVICEID_1, INSTANCEID_1);       // Service 1 as general Message-Service
    Service<uint32_t, STACKSIZE> service_2(SERVICEID_2, INSTANCEID_2);      // Service 2 as general 32-bit-Int-Service
    Service<char, STACKSIZE> service_3(SERVICEID_3, INSTANCEID_3);          // Service 3 as general char-Service

    ServiceBase* serviceList[3] = {&service_1, &service_2, &service_3};     // Array of ptr to the services 

    ServiceCluster<3> services(serviceList);

    std::cout<<"Instance-ID of Service"<<SERVICEID_1<<services.getService_byID(SERVICEID_1)<<std::endl;
}