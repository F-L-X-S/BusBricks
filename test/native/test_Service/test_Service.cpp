#include<Message.h>
#include<Content_stack.h>
#include<Service.h>
#include<ServiceCluster.h>
#include<unity.h>

#define STACKSIZE 3
#define SERVICEID_1 0x41    // ASCII "A"
#define SERVICEID_2 0x42    // ASCII "B"
#define SERVICEID_3 0x43    // ASCII "C"

#define INSTANCEID_1 0x61   // ASCII "a"
#define INSTANCEID_2 0x62   // ASCII "b"
#define INSTANCEID_3 0x63   // ASCII "c"

// ---------------------------------------Test example instance of abstract Content_stack-class---------------------------------------
void test_content_stack(void) {
    Content_stack<int, STACKSIZE> int_stack;                    // initialize int-stack for max 3 elements                                      

    // Increase Stacksize
    int8_t size = 0;                                           // initialize size-counter for added elements
    for (size_t i = 0; i <= STACKSIZE+3; i++)                   // try to add more elements than possible 
    {
        if (int_stack.addElement(i))                           // add int-elements with same value as index to stack 
        {
            size++;                                            // increase size-counter if element added successful
        }       
    }

    char message[100];
    sprintf(message, "Added %d elements, but initialized stack with max-size %d", size, STACKSIZE);
    TEST_ASSERT_EQUAL_MESSAGE(STACKSIZE, size, message);        // Check stacksize (should be max)

    // Get Item from Stack to check content 
    memset(message, '\0', sizeof(message));                    // re-initialize assert-message
    for (size_t i = 0; i < STACKSIZE+3; i++)
    {
        if (i<STACKSIZE)
        {
            TEST_ASSERT_EQUAL_MESSAGE(i, *(int_stack.getElement(i)), "indexed element should contain the same value as positive index");    
            TEST_ASSERT_EQUAL_MESSAGE(*(int_stack.getElement(i)), *(int_stack.getElement(i-STACKSIZE)), "pos. indexed element should be the same one as neg. indexed"); 
        }
        if (i>=STACKSIZE)
        {
            TEST_ASSERT_EQUAL_MESSAGE(nullptr, int_stack.getElement(i), "indexing out-of-bounds should return false"); 
        }
    }
    

    // Decrease Stacksize
    size = 0;                                                  // initialize size-counter for deleted elements
    for (size_t i = 0; i <= STACKSIZE+3; i++)                   // try to delete more elements than possible 
    {
        if (int_stack.deleteElement())                         // delete element on index 0
        {
            size++;                                            // increase size-counter if element deleted successful
        }       
    }

    memset(message, '\0', sizeof(message));                   // re-initialize assert-message
    sprintf(message, "deleted %d elements, but initialized stack with max-size %d", size, STACKSIZE);
    TEST_ASSERT_EQUAL_MESSAGE(STACKSIZE, size, message);        // Check stacksize (should be max)
}

// ---------------------------------------Test example instance of abstract Service-class---------------------------------------
void test_Service(void) {
    // Instantiate  simple Service for "Message"-content  
    Service<Message, STACKSIZE> message_service(SERVICEID_1, INSTANCEID_1);

    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    String sample_pdu = "\x01\x0F: PDU!";

    // Add Message-instances to receive-stack (Increase Stacksize)
    uint8_t size = 0;                                            // initialize size-counter for added elements
    for (size_t i = 0; i <= STACKSIZE+3; i++)                   // try to add more elements than possible 
    {
        // impart PDU to Service
        if (message_service.impart_pdu(&sample_pdu)) size ++;   // add Messages with text "index<i>", increase size-count
    }

    // Check number of imparted PDU
    char message[100];
    sprintf(message, "Added %d elements to rec-stack, but initialized stack with max-size %d", size, STACKSIZE);
    TEST_ASSERT_EQUAL_MESSAGE(STACKSIZE, size, message);        // Check stacksize (should be max)

    // Process Comm-stacks
    message_service.stackProcessing();


    // Add Message-instances to receive-stack (Increase Stacksize)
    Message expectedMsg;
    for (size_t i = 0; i <= STACKSIZE+3; i++)                   // try to add more elements than possible 
    {
        // impart PDU to Service
        message_service.impart_pdu(&sample_pdu);                // add Messages with text "index<i>"
    
        // Check response-PDUs
        expectedMsg=(i<STACKSIZE)? Message(&sample_pdu):Message();
        TEST_ASSERT_EQUAL_STRING_MESSAGE(expectedMsg.get_representation()->c_str(), message_service.get_response().c_str(), "Expected default-PDU as response-PDu for abstract Service");       
        message_service.clearResponse();
    }

}

// ---------------------------------------Test example instance of abstract ServiceCluster-class---------------------------------------
void test_ServiceCluster(void) {
    Service<Message, STACKSIZE> service_1(SERVICEID_1, INSTANCEID_1);       // Service 1 as general Message-Service
    Service<Message, STACKSIZE> service_2(SERVICEID_2, INSTANCEID_2);       // Service 2 as general 32-bit-Int-Service
    Service<Message, STACKSIZE> service_3(SERVICEID_3, INSTANCEID_3);       // Service 3 as general char-Service

    ServiceBase* serviceList[3] = {&service_1, &service_2, &service_3};     // Array of ptr to the services 

    ServiceCluster<3> services(serviceList);                                // Create a Service-Cluster from ptr-list to the associated services 

    // Test Get Service by ID
    ServiceBase* ret_service_1 = services.getService_byID(SERVICEID_1);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(SERVICEID_1,*ret_service_1->get_ServiceID(),
    "The ServiceID of the Service returned by ServiceClusters getService_byID-function does not match the searched ID");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(INSTANCEID_1,*ret_service_1->get_InstanceID(),
    "The InstanceID of the Service returned by ServiceClusters getService_byID-function does not match the searched ID");

    ServiceBase* ret_service_2 = services.getService_byID(SERVICEID_2);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(SERVICEID_2,*ret_service_2->get_ServiceID(),
    "The ServiceID of the Service returned by ServiceClusters getService_byID-function does not match the searched ID");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(INSTANCEID_2,*ret_service_2->get_InstanceID(),
    "The InstanceID of the Service returned by ServiceClusters getService_byID-function does not match the searched ID");

    ServiceBase* ret_service_3 = services.getService_byID(SERVICEID_3);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(SERVICEID_3,*ret_service_3->get_ServiceID(),
    "The ServiceID of the Service returned by ServiceClusters getService_byID-function does not match the searched ID");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(INSTANCEID_3,*ret_service_3->get_InstanceID(),
    "The InstanceID of the Service returned by ServiceClusters getService_byID-function does not match the searched ID");

    // Test Get Service by Position
    ret_service_1 = services.getService_byPos(0);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(SERVICEID_1,*ret_service_1->get_ServiceID(),
    "The ServiceID of the Service returned by ServiceClusters getService_byID-function does not match the searched ID");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(INSTANCEID_1,*ret_service_1->get_InstanceID(),
    "The InstanceID of the Service returned by ServiceClusters getService_byID-function does not match the searched ID");

    ret_service_2 = services.getService_byPos(1);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(SERVICEID_2,*ret_service_2->get_ServiceID(),
    "The ServiceID of the Service returned by ServiceClusters getService_byPos-function does not match the searched ID");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(INSTANCEID_2,*ret_service_2->get_InstanceID(),
    "The InstanceID of the Service returned by ServiceClusters getService_byPos-function does not match the searched ID");

    ret_service_3 = services.getService_byPos(2);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(SERVICEID_3,*ret_service_3->get_ServiceID(),
    "The ServiceID of the Service returned by ServiceClusters getService_byPos-function does not match the searched ID");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(INSTANCEID_3,*ret_service_3->get_InstanceID(),
    "The InstanceID of the Service returned by ServiceClusters getService_byPos-function does not match the searched ID");

}

// ---------------------------------------Run Tests---------------------------------------
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_content_stack);
    RUN_TEST(test_Service);
    RUN_TEST(test_ServiceCluster);
    UNITY_END();

    return 0;
}