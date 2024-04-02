#pragma once

#include <vector>
#include <sstream>
#include "PacketInfo.hpp"
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <spdlog/spdlog.h>
#include <netdb.h>
#include <arpa/inet.h>

std::vector<std::string> split (const std::string &s, char delim);
// template <typename T>
// std::vector <T> split(T string, T delim);

struct chat_message_info { 
    std::string command;
    std::string username;
    std::string secret;
    std::string display_name;
    std::string channel_id;
};

struct chat_message_info parse_message(char* msg_buffer);

std::string to_upper(std::string strToConvert);

char* extract_text_from_byte_array(uint8_t * byte_arr);

char* get_ip_addr_from_host(char* hostname,char* ip);