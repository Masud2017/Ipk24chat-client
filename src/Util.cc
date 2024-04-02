#include <chat_client/Util.hpp>

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}


std::string to_upper(std::string strToConvert)
{
   std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

    return strToConvert;
}

struct chat_message_info parse_message(char* buffer) {
    std::string msg_buffer(buffer);
    struct chat_message_info msg_info;
    std::vector<std::string> splitted_msg = split(msg_buffer, ' ');
    msg_info.command = to_upper(splitted_msg[0]);


    std::string auth ("/AUTH");
    std::string join ("/JOIN");
    std::string rename ("/RENAME");
    std::string bye ("/BYE");



    if (strcmp(msg_info.command.c_str(), auth.c_str()) == 0) {
        msg_info.username = splitted_msg[1];
        msg_info.display_name = splitted_msg[2];

        msg_info.secret = splitted_msg[3];


    } else if (strcmp(msg_info.command.c_str(), join.c_str()) == 0) {
        msg_info.channel_id = splitted_msg[1];

    } else if (strcmp(msg_info.command.c_str(), rename.c_str()) == 0) {
        msg_info.display_name = splitted_msg[1];
    }  else if (strcmp(msg_info.command.c_str(), bye.c_str()) == 0) {
        
    } 


    return msg_info;
}

char* extract_text_from_byte_array(uint8_t *byte_arr) {
    uint8_t *temp_buffer = (uint8_t *)malloc(sizeof(uint8_t)* 1024);
    bzero(temp_buffer,sizeof(temp_buffer));

    uint8_t prev_byte = 0xFF;
    int i = 0;
    while(true) {
        if (prev_byte == 0x00 &&  byte_arr[i] == 0x00) {
            break;
        }

        if (byte_arr[i] == 0x00) {
            temp_buffer[i] = 0x30;
        } else {
            temp_buffer[i] = byte_arr[i];
        }


        prev_byte = byte_arr[i];
        i++;
    }

    char* text_string = (char*)temp_buffer;
    text_string[strlen(text_string) - 1] = ' ';
    return text_string;
}

char* get_ip_addr_from_host(char* hostbuffer,char* ip) {
    struct addrinfo hint,*result;

    memset(&hint,0,sizeof(hint));
    hint.ai_family = AF_UNSPEC;

    int status = getaddrinfo(hostbuffer,ip, &hint,&result);

    struct addrinfo *temp = result ;
    void *addr;


    char* address_string = (char*) malloc(sizeof(char)*INET6_ADDRSTRLEN);
    addr = &((struct sockaddr_in*)temp->ai_addr)->sin_addr;
    inet_ntop(temp->ai_family,addr,address_string,INET6_ADDRSTRLEN);
    printf("\n\n%s\n\n",address_string);

    return address_string;
}