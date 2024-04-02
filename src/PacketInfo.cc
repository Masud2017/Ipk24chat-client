#include <chat_client/PacketInfo.hpp>


// Message type
const uint8_t CONFIRM = 0x00;
const uint8_t REPLY = 0x01;
const uint8_t AUTH = 0x02;
const uint8_t JOIN = 0x03;
const uint8_t MSG = 0x04;
const uint8_t err = 0xFE;
const uint8_t BYE = 0xFF;

uint8_t result;

uint16_t MessageId; // holds 2 bytes of information

char* username = (char*)malloc(sizeof(char) * 20);
char* channel_id = (char*)malloc(sizeof(char) * 20);
char* secret = (char*)malloc(sizeof(char) * 128);
char* display =(char*)malloc(sizeof(char) * 20);
char* msg_content =(char*)malloc(sizeof(char) * 1400);


std::string get_message_type_str (MessageType msg_type) {
    switch (msg_type) {
        case TYPE_CONFIRM:
            return "CONFIRM";
            break;
        case TYPE_REPLY:
            return "REPLY";
            break;
        case TYPE_AUTH:
            return "AUTH";
            break;
        case TYPE_JOIN:
            return "JOIN";
            break;
        case TYPE_MSG:
            return "MSG";
            break;
        case TYPE_ERR:
            return "ERR";
            break;
        case TYPE_BYE:
            return "BYE";
            break;
    }

    return "BYE";
}


MessageType get_message_type_str_from_msg_type (std::string msg_type_str) {
    if (msg_type_str == "CONFIRM") {
        return MessageType::TYPE_CONFIRM;
    } else if (msg_type_str == "REPLY") {
        return MessageType::TYPE_REPLY;
    } else if (msg_type_str == "AUTH") {
        return MessageType::TYPE_AUTH;
    } else if (msg_type_str == "JOIN") {
        return MessageType::TYPE_JOIN;
    } else if (msg_type_str == "MSG") {
        return MessageType::TYPE_MSG;
    } else if (msg_type_str == "ERR") {
        return MessageType::TYPE_ERR;
    } else if (msg_type_str == "BYE") {
        return MessageType::TYPE_BYE;
    }

    return MessageType::TYPE_BYE;
}



const std::string is = "IS";
const std::string as = "AS";
const std::string use = "USING";
const std::string from = "from";
const std::string ok = "OK";
const std::string nok = "NOK";
