#include <gtest/gtest.h>
#include <chat_client/Util.hpp>
#include <spdlog/spdlog.h>
#include <chat_client/PacketHandler.hpp>

// chat_client::PacketHandler packetHandler;


TEST(UtilTest, TestParseMessage) {
    char* msg = "/AUTH xdolez97 dave 00f1b575-44ff-464d-ac90-a448a394e1d2";
    struct chat_message_info expected {
        "/AUTH",
        "xdolez97",
        "00f1b575-44ff-464d-ac90-a448a394e1d2",
        "dave"
    };

    struct chat_message_info actual = parse_message(msg);

    ASSERT_EQ(expected.command , "/AUTH");
    ASSERT_EQ(expected.username , "xdolez97");
    ASSERT_EQ(expected.secret, "00f1b575-44ff-464d-ac90-a448a394e1d2");
    ASSERT_EQ(expected.display_name , "dave");


    char* msg_join = "/join dusername display_use";
    struct chat_message_info expected_join {
        "/JOIN",
        "",
        "",
        "",
        "dusername"
    };

    struct chat_message_info actual_join = parse_message(msg_join);

    ASSERT_EQ(expected_join.command , actual_join.command);
    ASSERT_EQ(expected_join.channel_id , actual_join.channel_id);
}

TEST(UtilTest, TestExtractTextFromByteArray) {
    chat_client::PacketHandler packetHandler;
    uint8_t *buffer = packetHandler.auth_udp(3,"user","display_name","secret");
    char* expected = "user0display_name0secret ";

    std::string actual = extract_text_from_byte_array(buffer+3);
    std::string expected_str(expected);


    ASSERT_EQ(expected_str,actual);

}

TEST(UtilTest, TestGetIpAddrFromHost) {
    std::string expected("147.229.8.244");
    std::string actual = std::string(get_ip_addr_from_host("anton5.fit.vutbr.cz","4567"));

    spdlog::info(actual);

    ASSERT_TRUE(strcmp((char*)expected.c_str(),(char*)actual.c_str()) == 0);
}