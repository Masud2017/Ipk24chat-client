#include <chat_client/PacketHandler.hpp>


uint8_t* chat_client::PacketHandler::confirm_udp(uint16_t ref_msg_id) {
    uint8_t *buffer = (uint8_t*)malloc(sizeof(uint8_t) * 3);

    buffer[0] = CONFIRM;
    buffer[1] = (ref_msg_id >> 8) & 0xFF; // higher segment of the message id
    buffer[2] = ref_msg_id & 0xFF; // lower segment of the message id

    return buffer;
}

uint8_t* chat_client::PacketHandler::reply_udp(uint16_t msg_id, uint8_t result, uint16_t ref_msg_id, char* message_content) {
    uint8_t *buffer = (uint8_t*)malloc(sizeof(uint8_t) * 4000);

    buffer[0] = REPLY;
    buffer[1] = (msg_id >> 8) & 0xFF;
    buffer[2] = msg_id & 0xFF;
    buffer[3] = result;
    buffer[4] = (ref_msg_id >> 8) & 0xFF;
    buffer[5] = ref_msg_id & 0xFF;


    size_t msg_len = strlen(message_content);

    memcpy(buffer + 6, username, msg_len);
    buffer[3 + msg_len] = 0x00;

    return buffer;
}

uint8_t* chat_client::PacketHandler::auth_udp(uint16_t msg_id, char* username, char* display_name, char* secret) {
    uint8_t *buffer = (uint8_t*)malloc(sizeof(uint8_t) * 4000);

    buffer[0] = AUTH;
    buffer[1] = (msg_id >> 8) & 0xFF;    
    buffer[2] = msg_id & 0xFF;

    size_t username_len = strlen(username);
    size_t display_name_len = strlen(display_name);
    size_t secret_len = strlen(secret);

    memcpy(buffer + 3, username, username_len);
    buffer[3 + username_len] = 0x00;

    memcpy(buffer + 4 + username_len, display_name, display_name_len);
    buffer[4 + username_len + display_name_len] = 0x00;

    memcpy(buffer + 5 + username_len + display_name_len, secret, secret_len);
    buffer[5 + username_len + display_name_len + secret_len] = 0x00;

    return buffer;
}

uint8_t* chat_client::PacketHandler::join_udp(uint16_t msg_id, char* channel_id, char* display_name) {
    uint8_t *buffer = (uint8_t*)malloc(sizeof(uint8_t) * 4000);

    buffer[0] = JOIN;
    buffer[1] = (msg_id >> 8) & 0xFF;
    buffer[2] = msg_id & 0xFF;



    size_t channel_id_len = strlen(channel_id);
    size_t display_name_len = strlen(display_name);

    memcpy(buffer + 3, channel_id, channel_id_len);
    buffer[3 + channel_id_len] = 0x00;

    memcpy(buffer + 4 + channel_id_len, display_name, display_name_len);
    buffer[4 + channel_id_len + display_name_len] = 0x00;
    
    return buffer;
}

uint8_t* chat_client::PacketHandler::msg_udp(uint16_t msg_id,char* display_name, char* message_content) {
    uint8_t *buffer = (uint8_t*)malloc(sizeof(uint8_t) * 4000);

    buffer[0] = MSG;
    buffer[1] = (msg_id >> 8) & 0xFF;
    buffer[2] = msg_id & 0xFF;


    size_t display_name_len = strlen(display_name);
    size_t msg_content_len = strlen(message_content);


    memcpy(buffer + 3, display_name, display_name_len);
    buffer[3 + display_name_len] = 0x00;

    memcpy(buffer + 4 + display_name_len, message_content, msg_content_len);
    buffer[4 + display_name_len + msg_content_len] = 0x00;



    return buffer;
}

uint8_t* chat_client::PacketHandler::err_udp(uint16_t msg_id, char* display_name, char* msg_content) {
    uint8_t *buffer = (uint8_t*)malloc(sizeof(uint8_t) * 4000);

    buffer[0] = err;
    buffer[1] = (msg_id >> 8) & 0xFF;
    buffer[2] = msg_id & 0xFF;

    strcpy((char*)(buffer + 3),display_name);
    buffer [3 + sizeof(display_name)] = 0x00;

    strcpy((char*)(buffer + 3 + sizeof(display_name) + 1),msg_content);
    buffer [3 + sizeof(display_name) + 1 + sizeof(msg_content)] = 0x00;

    return 0;
}

uint8_t* chat_client::PacketHandler::bye_udp(uint16_t msg_id) {
    uint8_t *buffer = (uint8_t*)malloc(sizeof(uint8_t) * 3);

    buffer[0] = BYE;

    buffer[1] = (msg_id >> 8) & 0xFF;
    buffer[2] = msg_id & 0xFF;

    return buffer;
}


// function definations for tcp protocol
std::string chat_client::PacketHandler::reply_tcp(std::string status, std::string content) {
    std::stringstream string_stream;

    string_stream << get_message_type_str(MessageType::TYPE_REPLY) << " " << status << " " << is << " " << content << "\r\n";

    return string_stream.str();
}

std::string chat_client::PacketHandler::auth_tcp(std::string username, std::string display_name, std::string secret) {
    char* buff = (char*)malloc(sizeof(char) * 2000);

    sprintf(buff,"%s %s AS %s USING %s",(char*)get_message_type_str(MessageType::TYPE_AUTH).c_str(),(char*)username.c_str(),(char*)display_name.c_str(),(char*)secret.c_str());
    strcat(buff,"\r\n");
    


    return buff;
}

std::string chat_client::PacketHandler::join_tcp(std::string channel_id, std::string display_name) {
    char* buff = (char*)malloc(sizeof(char) * 2000);

    sprintf(buff,"%s %s AS %s",(char*)get_message_type_str(MessageType::TYPE_JOIN).c_str(),(char*)channel_id.c_str(),(char*)display_name.c_str());
    strcat(buff,"\r\n");

    return buff;
}

std::string chat_client::PacketHandler::msg_tcp(std::string display_name, std::string content) {
    char* buff = (char*)malloc(sizeof(char) * 2000);

    sprintf(buff,"%s FROM %s IS %s",(char*)get_message_type_str(MessageType::TYPE_MSG).c_str(),(char*)display_name.c_str(),(char*)content.c_str());
    strcat(buff,"\r\n");
    
    return buff;
}

std::string chat_client::PacketHandler::err_tcp(std::string display_name, std::string content) {
    std::stringstream string_stream;

    string_stream << get_message_type_str(MessageType::TYPE_ERR) << " " << "FROM" << " " << display_name << " " << is << " " << content << "\r\n";

    return string_stream.str();
}

std::string chat_client::PacketHandler::bye_tcp() {
    std::stringstream string_stream;

    string_stream << get_message_type_str(MessageType::TYPE_BYE) << "\r\n";
    char* buff = (char*)malloc(sizeof(char) * 2000);

    sprintf(buff,"%s",(char*)get_message_type_str(MessageType::TYPE_BYE).c_str());
    strcat(buff,"\r\n");
    

    return buff;
}

struct UdpData chat_client::PacketHandler::read_udp_msg(uint8_t* buffer) {
    struct UdpData udp_data;

    if (buffer[0] == CONFIRM) {
        udp_data.msg_type = MessageType::TYPE_CONFIRM;

        // udp_data.ref_msg_id = ((uint16_t)buffer[1]);
        // udp_data.ref_msg_id |= buffer[2] << 8;
        udp_data.msg_id = (uint16_t) buffer[1] << 8;
        udp_data.msg_id |= buffer[2];

    } else if (buffer[0] == REPLY) {
        udp_data.msg_type = MessageType::TYPE_REPLY;

        // udp_data.msg_id = ((uint16_t)buffer[1]);
        // udp_data.msg_id |= buffer[2] << 8;
        udp_data.msg_id = (uint16_t) buffer[1] << 8;
        udp_data.msg_id |= buffer[2];

        udp_data.result = buffer[3];

        // udp_data.ref_msg_id = ((uint16_t)buffer[4]);
        // udp_data.ref_msg_id |= buffer[5] <<8;
        udp_data.ref_msg_id = (uint16_t) buffer[4] << 8;
        udp_data.ref_msg_id |= buffer[5];

        udp_data.msg_content = extract_text_from_byte_array(buffer+6);

    } else if (buffer[0] == AUTH) { // not needed
        udp_data.msg_type = MessageType::TYPE_AUTH;

        udp_data.msg_id = ((uint16_t)buffer[1]);
        udp_data.msg_id |= buffer[2] << 8;

        char* payload = extract_text_from_byte_array(buffer+3);


    } else if (buffer[0] == JOIN) {
        udp_data.msg_type = MessageType::TYPE_JOIN;
        // udp_data.msg_id = ((uint16_t)buffer[1]);
        // udp_data.msg_id |= buffer[2] << 8;
        udp_data.msg_id = (uint16_t) buffer[1] << 8;
        udp_data.msg_id |= buffer[2];

    } else if (buffer[0] == MSG) {
        udp_data.msg_type = MessageType::TYPE_MSG;
        // udp_data.msg_id = ((uint16_t)buffer[1]);
        // udp_data.msg_id |= buffer[2] << 8;
        udp_data.msg_id = (uint16_t) buffer[1] << 8;
        udp_data.msg_id |= buffer[2];

        char* payload = extract_text_from_byte_array(buffer + 3);

        std::vector<std::string> splitted_payload = split(payload,'0');

        // udp_data.display_name = (char*) splitted_payload[0].c_str();
        udp_data.display_name = (char*)malloc(sizeof(char) * strlen((char*) splitted_payload[0].c_str()));
        udp_data.msg_content = (char*)malloc(sizeof(char) * strlen((char*) splitted_payload[1].c_str()));

        strcpy(udp_data.display_name,splitted_payload[0].c_str());
        strcpy(udp_data.msg_content,splitted_payload[1].c_str());

        // udp_data.msg_content = (char*) splitted_payload[1].c_str();
        

    } else if (buffer[0] == err) {
        spdlog::info("Error got ");
        udp_data.msg_type = MessageType::TYPE_ERR;

        // udp_data.msg_id = ((uint16_t)buffer[1]);
        // udp_data.msg_id |= buffer[2] << 8;
        udp_data.msg_id = (uint16_t) buffer[1] << 8;
        udp_data.msg_id |= buffer[2];

        char* payload = extract_text_from_byte_array(buffer + 3);

        std::vector<std::string> splitted_payload = split(payload,'0');

        udp_data.display_name = (char*) splitted_payload[0].c_str();
        char* msg_content = (char*)malloc(sizeof(char)*1024);

        strncpy(msg_content,payload + (strlen(udp_data.display_name)+1),1024);
        udp_data.msg_content = msg_content;



    } else if (buffer[0] == BYE) {
        // done
        udp_data.msg_type = MessageType::TYPE_BYE;

        // udp_data.msg_id = ((uint16_t)buffer[1]);
        // udp_data.msg_id |= buffer[2] << 8;
        udp_data.msg_id = (uint16_t) buffer[1] << 8;
        udp_data.msg_id |= buffer[2];
    }


    return udp_data;
}
//edit need 
struct TcpData chat_client::PacketHandler::read_tcp_msg(std::string buffer) {
    struct TcpData tcp_data;

    std::vector<std::string> splitted_str_arr = split(buffer,' ');
    tcp_data.msg_type = get_message_type_str_from_msg_type(splitted_str_arr[0]);
    int splitted_size = splitted_str_arr.size();



    if (splitted_str_arr[0] == "REPLY") { // done
        tcp_data.status = splitted_str_arr[1];
        std::stringstream ss;
        
        for (int i = 0; i < splitted_str_arr.size(); i++)  {
            if (i == 0 || i == 1 || i == 2) {
                continue;
            }
            ss << splitted_str_arr[i] << " ";
        }

        tcp_data.msg_content  = ss.str();
    } else if (splitted_str_arr[0] == "AUTH") { // not needed
        tcp_data.username = splitted_str_arr[1];
        tcp_data.display_name = splitted_str_arr[3];
        tcp_data.secret = splitted_str_arr[5];
    } else if (splitted_str_arr[0] == "JOIN") { // not needed 
        tcp_data.channel_id = splitted_str_arr[1];
        tcp_data.display_name = splitted_str_arr[3];
    } else if (splitted_str_arr[0] == "MSG") { // done
        tcp_data.display_name = splitted_str_arr[2];

        std::stringstream ss;
        
        for (int i = 0; i < splitted_str_arr.size(); i++)  {
            if (i == 0 || i == 1 || i == 2 || i == 3) {
                continue;
            }
            ss << splitted_str_arr[i] << " ";
        }

        tcp_data.msg_content = ss.str();
    } else if (splitted_str_arr[0] == "ERR") {
        tcp_data.display_name = splitted_str_arr[2];

        std::stringstream ss;
        
        for (int i = 0; i < splitted_str_arr.size(); i++)  {
            if (i == 0 || i == 1 || i == 2 || i == 3) {
                continue;
            }
            ss << splitted_str_arr[i] << " ";
        }

        tcp_data.msg_content = ss.str();

        // tcp_data.msg_content = splitted_str_arr[4];
    }

    return tcp_data;
}