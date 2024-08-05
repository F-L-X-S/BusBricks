#include<Message_service.h>
#include<Message.h>
#include<unity.h>

#define STACKSIZE 3
#define FUNCTIONCODE 0x6D // ASCII: "m"
#define INSTANCE_ID 0xF


// ---------------------------------------Test example instance of abstract Service-class constructed by content-struct---------------------------------------
void test_Message_from_content(void) {
    //Create a sample message-content with Sender 0x1 and Receiver 0xF
    Message_content_t msg_content;
    msg_content.sender_id = 0x1;
    msg_content.receiver_id = 0xF;
    msg_content.msg_text= "Hello";  
    Message msg_from_content(&msg_content);

    // Check String-representation of created object
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Sender: 0x01\x09\x09Receiver: 0x0F\nHello\n", msg_from_content.to_string().c_str(),\
    "Message-Object created with msg_from_pdu-constructor \
    returns incorrect string-representation ");     

    // Check PDU of created object
    TEST_ASSERT_EQUAL_STRING_MESSAGE("\x01\x0F:Hello", msg_from_content.get_representation()->c_str(),\
    "Message-Object created with msg_from_pdu-constructor \
    returns incorrect pdu");     
}

// ---------------------------------------Test example instance of abstract Service-class constructed by PDU---------------------------------------
void test_Message_from_pdu(void) {

    // Create a sample PDU with Sender 0x1 and Receiver 0xF
    String sample_pdu = "\x01\x0F:Hello";
    Message msg_from_pdu(&sample_pdu);


    // Check String-representation of created object
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Sender: 0x01\x09\x09Receiver: 0x0F\nHello\n", msg_from_pdu.to_string().c_str(),\
    "Message-Object created with msg_from_pdu-constructor \
    returns incorrect string-representation ");     

    // Check PDU of created object
    TEST_ASSERT_EQUAL_STRING_MESSAGE("\x01\x0F:Hello", msg_from_pdu.get_representation()->c_str(),\
    "Message-Object created with msg_from_pdu-constructor \
    returns incorrect pdu");     
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_Message_from_content);
    RUN_TEST(test_Message_from_pdu);
    UNITY_END();

    return 0;
}