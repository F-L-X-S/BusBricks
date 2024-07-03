#include<Message_service.h>
#include<Message.h>
#include<unity.h>

#define STACKSIZE 3
#define FUNCTIONCODE 0x6D // ASCII: "m"
#define INSTANCE_ID 0xF


// ---------------------------------------Test example instance of abstract Service-class---------------------------------------
void test_Message(void) {
    //Create a sample message-content with Sender 0x1 and Receiver 0xF
    Message_content_t msg_content;
    msg_content.sender_id = 0x1;
    msg_content.receiver_id = 0xF;
    strcpy(msg_content.msg_text, "Hello");  
    msg_content.txt_size = 5; 
    Message msg_from_content(&msg_content);

    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    char sample_pdu[] = {0x1, 0xF, 'H', 'e', 'l', 'l', 'o'};
    uint8_t pdu_size = 7; // Size of PDU
    Message msg_from_pdu(sample_pdu, &pdu_size);


    // Check String-representation of both objects
    TEST_ASSERT_EQUAL_STRING_MESSAGE(msg_from_pdu.to_string(), msg_from_content.to_string(),\
    "Message-Object created with msg_from_content-constructor \
    should return same string-representation as Message-Object created with msg_from_pdu-constructor");     

    // Check PDU of both objects 
    TEST_ASSERT_EQUAL_STRING_MESSAGE(msg_from_pdu.get_pdu(), msg_from_content.get_pdu(),\
    "Message-Object created with msg_from_content-constructor \
    should return same pdu as Message-Object created with msg_from_pdu-constructor");     

}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_Message);
    UNITY_END();

    return 0;
}