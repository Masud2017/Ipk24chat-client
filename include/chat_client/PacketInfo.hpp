#pragma once

#include <stdint.h>
#include <malloc.h>
#include <map>
#include <iostream>
#include<string>


// Message type
extern const uint8_t CONFIRM;
extern const uint8_t REPLY;
extern const uint8_t AUTH;
extern const uint8_t JOIN;
extern const uint8_t MSG;
extern const uint8_t err;
extern const uint8_t BYE;

extern uint8_t result;

extern uint16_t MessageId; // holds 2 bytes of information

extern char* username;
extern char* channel_id;
extern char* secret;
extern char* display;
extern char* msg_content;

// Message type FMS
enum MessageType {
    TYPE_CONFIRM,
    TYPE_REPLY,
    TYPE_AUTH,
    TYPE_JOIN,
    TYPE_MSG,
    TYPE_ERR,
    TYPE_BYE
};

extern std::string get_message_type_str (MessageType msg_type);


extern MessageType get_message_type_str_from_msg_type (std::string msg_type_str);


extern const std::string is;
extern const std::string as;
extern const std::string use;
extern const std::string from;
extern const std::string ok;
extern const std::string nok;


struct UdpData {
    MessageType msg_type;
    uint16_t msg_id;
    uint16_t ref_msg_id;
    char* display_name;
    char* secret;
    char* channel_id;
    char* msg_content;
    uint8_t result;
};

struct TcpData {
    MessageType msg_type;
    std::string display_name;
    std::string secret;
    std::string msg_content;
    std::string msg_id;
    std::string status;// only gets populated if the message is about reply
    std::string username;
    std::string channel_id;
};