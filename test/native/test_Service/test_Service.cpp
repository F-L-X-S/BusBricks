#include<Content_stack.h>
#include<unity.h>

// Try to add more elements to an Content-Stack than the stack-size was initialized with 
void test_content_stack(void) {
    const uint8_t max_size = 3;                                // Max number of elements in stack 
    Content_stack<int, max_size> int_stack;                    // initialize int-stack for max 3 elements                                      

    // Increase Stacksize
    int8_t size = 0;                                           // initialize size-counter for added elements
    for (size_t i = 0; i <= max_size+3; i++)                   // try to add more elements than possible 
    {
        if (int_stack.addElement(i))                           // add int-elements with same value as index to stack 
        {
            size++;                                            // increase size-counter if element added successful
        }       
    }

    char message[100];
    sprintf(message, "Added %d elements, but initialized stack with max-size %d", size, max_size);
    TEST_ASSERT_EQUAL_MESSAGE(max_size, size, message);        // Check stacksize (should be max)

    // Get Item from Stack to check content 
    memset(message, '\0', sizeof(message));                    // re-initialize assert-message
    for (size_t i = 0; i < max_size+3; i++)
    {
        if (i<max_size)
        {
            TEST_ASSERT_EQUAL_MESSAGE(i, int_stack.getElement(i), "indexed element should contain the same value as positive index");    
            TEST_ASSERT_EQUAL_MESSAGE(int_stack.getElement(i), int_stack.getElement(i-max_size), "pos. indexed element should be the same one as neg. indexed"); 
        }
        if (i>=max_size)
        {
            TEST_ASSERT_EQUAL_MESSAGE(false, int_stack.getElement(i), "indexing out-of-bounds should return false"); 
        }
    }
    

    // Decrease Stacksize
    size = 0;                                                  // initialize size-counter for deleted elements
    for (size_t i = 0; i <= max_size+3; i++)                   // try to delete more elements than possible 
    {
        if (int_stack.deleteElement())                         // delete element on index 0
        {
            size++;                                            // increase size-counter if element deleted successful
        }       
    }

    memset(message, '\0', sizeof(message));                   // re-initialize assert-message
    sprintf(message, "deleted %d elements, but initialized stack with max-size %d", size, max_size);
    TEST_ASSERT_EQUAL_MESSAGE(max_size, size, message);        // Check stacksize (should be max)
}


int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_content_stack);
    UNITY_END();

    return 0;
}