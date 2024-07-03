#include<Message_service.h>
#include<Message.h>
#include<Content.h>
#include<unity.h>

#define STACKSIZE 3
#define FUNCTIONCODE 0x6D // ASCII: "m"
#define INSTANCE_ID 0xF


// ---------------------------------------Test example instance of abstract Service-class---------------------------------------
void test_Message(void) {
    //Create a sample Message from message-content with Sender 0x1 and Receiver 0xF
    Message_content_t msg_content;
    msg_content.sender_id = 0x1;
    msg_content.receiver_id = 0xF;
    strcpy(msg_content.msg_text, "Hello");   
    Message msg_from_content(&msg_content);

    // // Create a sample Message from PDU with Sender 0x1 and Receiver 0xF
    char sample_pdu[] = {0x1, 0xF, 'H', 'e', 'l', 'l', 'o'};
    uint8_t pdu_size = 7; // Size of PDU
    Message msg_from_pdu(sample_pdu, &pdu_size);

    TEST_ASSERT_EQUAL_STRING_ARRAY_MESSAGE(*msg_from_pdu.to_string(), *msg_from_content.to_string(), MAXPDUSIZE,"Message-Objects created with msg_from_content-constructor \
    should be equal to Message-Objects created with msg_from_pdu-constructor");       
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_Message);
    UNITY_END();

    return 0;
}