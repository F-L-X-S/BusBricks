#include<Content_stack.h>
#include<unity.h>

// Try to add more elements to an Content-Stack than the stack-size was initialized with 
void test_add_elements_till_max(void) {
    const uint8_t max_size = 3;                                // Max number of elements in stack 
    Content_stack<int, max_size> int_stack;                    // initialize int-stack for max 3 elements
    uint8_t size = 0;
    for (size_t i = 1; i <= max_size+3; i++)                   // try to add more elements than possible 
    {
        if (int_stack.addElement(i))
        {
            size++;                                            // increase size-counter if element added successful
        }       
    }
    char message[100];
    sprintf(message, "Added %d elements, but initialized stack with max-size %d", size, max_size);
    TEST_ASSERT_EQUAL_MESSAGE(max_size, size, message);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_add_elements_till_max);
    UNITY_END();

    return 0;
}