#include<Content_stack.h>
#include<Service.h>
#include<Message.h>
#include<unity.h>

#define STACKSIZE 3
#define FUNCTIONCODE 0x6D // ASCII: "m"
#define INSTANCE_ID 0xF

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
            TEST_ASSERT_EQUAL_MESSAGE(i, int_stack.getElement(i), "indexed element should contain the same value as positive index");    
            TEST_ASSERT_EQUAL_MESSAGE(int_stack.getElement(i), int_stack.getElement(i-STACKSIZE), "pos. indexed element should be the same one as neg. indexed"); 
        }
        if (i>=STACKSIZE)
        {
            TEST_ASSERT_EQUAL_MESSAGE(false, int_stack.getElement(i), "indexing out-of-bounds should return false"); 
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
    Service<Message, STACKSIZE> message_service(FUNCTIONCODE, INSTANCE_ID);

    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    char sample_pdu[] = {0x1, 0xF, 'i', 'n', 'd', 'e', 'x', '!'};
    uint8_t pdu_size = 8; // Size of PDU


    // Add Message-instances to receive-stack (Increase Stacksize)
    int8_t size = 0;                                           // initialize size-counter for added elements
    for (size_t i = 0; i <= STACKSIZE+3; i++)                  // try to add more elements than possible 
    {
        sample_pdu[7] = i;                                     // change msg-text to "index<i>"
        if (message_service.impart_pdu(sample_pdu, &pdu_size)) // add Messages with text "index<i>"
        {
            size++;                                            // increase size-counter if message added successful to receive stack
        }
        
        // Check response-PDU after each impart
        char empty_pdu[MAXPDUSIZE];
        Message empty_msg;
        TEST_ASSERT_EQUAL_STRING_MESSAGE(empty_msg.get_pdu(), message_service.get_response(), "Expected default-PDU as response-PDu for abstract Service");       
    }
    char message[100];
    sprintf(message, "Added %d elements to rec-stack, but initialized stack with max-size %d", size, STACKSIZE);
    TEST_ASSERT_EQUAL_MESSAGE(STACKSIZE, size, message);        // Check stacksize (should be max)
}

// ---------------------------------------Run Tests---------------------------------------
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_content_stack);
    RUN_TEST(test_Service);
    UNITY_END();

    return 0;
}