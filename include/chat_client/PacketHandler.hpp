#pragma once

#include "PacketInfo.hpp"
#include "Util.hpp"

#include <string.h>
#include <sstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <vector>


namespace chat_client {
    /**
     * <h1>PacketHandler</h1>
     * This class crafts data in a way that the server will accept
     * PacketHandler contains both the udp and tcp packet structure constructor
    */
    class PacketHandler {
        private:
            int counter;
        public:

            uint8_t* confirm_udp(uint16_t ref_msg_id);
            uint8_t* reply_udp(uint16_t msg_id, uint8_t result, uint16_t ref_msg_id, char* message_content);
            uint8_t* auth_udp(uint16_t msg_id, char* username, char* display_name, char* secret);
            uint8_t* join_udp(uint16_t msg_id, char* channel_id, char* display_name);
            uint8_t* msg_udp(uint16_t msg_id,char* display_name, char* message_content);
            uint8_t* err_udp(uint16_t msg_id, char* display_name, char* msg_content);
            uint8_t* bye_udp(uint16_t msg_id);

            std::string reply_tcp(std::string status, std::string content);
            std::string auth_tcp(std::string username, std::string display_name, std::string secret);
            std::string join_tcp(std::string channel_id, std::string display_name);
            std::string msg_tcp(std::string display_name, std::string content);
            std::string err_tcp(std::string display_name, std::string content);
            std::string bye_tcp();

            struct UdpData read_udp_msg(uint8_t* buffer);
            struct TcpData read_tcp_msg(std::string buffer);
    };
}