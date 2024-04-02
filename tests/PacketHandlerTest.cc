#include <chat_client/PacketHandler.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

chat_client::PacketHandler packetHandler;

TEST(PacketHandlerTest, TestAuthTcp)
{
    
    const std::string expected("AUTH xdolez97 AS dave USING 00f1b575-44ff-464d-ac90-a448a394e1d2\r\n") ;
    const std::string actual = packetHandler.auth_tcp("xdolez97","dave","00f1b575-44ff-464d-ac90-a448a394e1d2");
    ASSERT_EQ(expected, actual);
}

TEST(PacketHandlerTest, TestReplyTcp)
{
    
    const std::string expected("REPLY OK IS msg_content\r\n") ;
    const std::string actual = packetHandler.reply_tcp("OK","msg_content");
    ASSERT_EQ(expected, actual);
}

TEST(PacketHandlerTest, TestJoinTcp)
{
    
    const std::string expected("JOIN discord.channel AS display_name\r\n") ;
    const std::string actual = packetHandler.join_tcp("discord.channel","display_name");
    ASSERT_EQ(expected, actual);
}

TEST(PacketHandlerTest, TestMsgTcp)
{
    
    const std::string expected("MSG FROM dave IS titi\r\n") ;
    const std::string actual = packetHandler.msg_tcp("dave","titi");
    ASSERT_EQ(expected, actual);
}

TEST(PacketHandlerTest, TestErrTcp)
{
    
    const std::string expected("ERR FROM DisplayName IS MessageContent\r\n") ;
    const std::string actual = packetHandler.err_tcp("DisplayName","MessageContent");
    ASSERT_EQ(expected, actual);
}

TEST(PacketHandlerTest, TestByeTcp)
{
    
    const std::string expected("BYE\r\n") ;
    const std::string actual = packetHandler.bye_tcp();
    ASSERT_EQ(expected, actual);
}

TEST(PacketHandlerTest, TestAuthUdp)
{
    
    typedef uint8_t bt;
    bt* expected_buf = (bt*) malloc(sizeof(bt) * 4024);
    expected_buf[0] = 0x02;
    expected_buf[1] = 0x02;
    expected_buf[2] = 0x00;

    char* username = "xdt2";
    char* display_name = "dave";
    char* secret = "330f-34ckkd-34";


    size_t username_len = strlen(username);
    size_t display_name_len = strlen(display_name);
    size_t secret_len = strlen(secret);

    // Copy username, display_name, and secret into expected_buf
    memcpy(expected_buf + 3, username, username_len);
    expected_buf[3 + username_len] = 0x00;

    memcpy(expected_buf + 4 + username_len, display_name, display_name_len);
    expected_buf[4 + username_len + display_name_len] = 0x00;

    memcpy(expected_buf + 5 + username_len + display_name_len, secret, secret_len);
    expected_buf[5 + username_len + display_name_len + secret_len] = 0x00;




    bt* actual_buf = packetHandler.auth_udp(2,"xdt2","dave","330f-34ckkd-34");
    int arr_sz = sizeof(expected_buf)/sizeof(expected_buf[0]);

   

    uint8_t prev_byte;
    int i = 0;
    while(true) {
        if (prev_byte == 0x00 && expected_buf[i] == 0x00) {
            printf("data ended \n");
            break;
        }
        printf("%02x",expected_buf[i]);
        prev_byte = expected_buf[i];
        i++;
    }

    ASSERT_EQ(expected_buf[0], actual_buf[0]);
    ASSERT_EQ(expected_buf[1], actual_buf[1]);
    ASSERT_EQ(expected_buf[2], actual_buf[2]);
    ASSERT_EQ(expected_buf[3], actual_buf[3]);
}



TEST(PacketHandlerTest, TestConfirmUdp) {
    uint8_t expected_buffer[] = {0x00,0x05,0x00};
    uint8_t *actual_buffer = packetHandler.confirm_udp(5);

    for (int i = 0; i < 100; i++) {
        printf("%02x",actual_buffer[i]);
    }

    ASSERT_EQ(expected_buffer[0], actual_buffer[0]);
    ASSERT_EQ(expected_buffer[1], actual_buffer[1]);
    ASSERT_EQ(expected_buffer[2], actual_buffer[2]);


}

TEST(PacketHandlerTest, TestReplyUdp) {
    uint8_t buffer[] = {0x00,0x02,0x00};
    uint8_t *actual = packetHandler.confirm_udp(2);

    UdpData data = packetHandler.read_udp_msg(actual);
    printf("Value of ref msg : %d",data.ref_msg_id);

    ASSERT_EQ(buffer[0],actual[0]);
    ASSERT_EQ(buffer[1],actual[1]);
    ASSERT_EQ(buffer[2],actual[2]);

    
}


TEST(PacketHandlerTest, TestMsgUdp) {
    uint8_t buffer[] = {0x04,0x02,0x00};

    uint8_t *actual = packetHandler.msg_udp(2,"display_name","msg_content");

    char* expected_extracted_msg = "display_name0msg_content ";
    char* actual_extracted_msg = extract_text_from_byte_array(actual+3);


    UdpData data = packetHandler.read_udp_msg(actual);
    printf("Value of ref msg : %d",data.ref_msg_id);

    ASSERT_EQ(buffer[0],actual[0]);
    ASSERT_EQ(buffer[1],actual[1]);
    ASSERT_EQ(buffer[2],actual[2]);
    ASSERT_EQ(std::string(expected_extracted_msg),std::string(actual_extracted_msg));
    ASSERT_TRUE(strcmp(expected_extracted_msg,actual_extracted_msg) == 0);    
}

TEST(PacketHandlerTest, TestUdpReader) {

}