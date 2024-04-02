#include <chat_client/UdpCommunicator.hpp>

chat_client::UdpCommunicator::UdpCommunicator(std::string host,int port,int retry_count, int timeout) {
    this->udp = new chat_client::NetWork::Udp((char*)host.c_str(),port);
    this->packetHandler = new chat_client::PacketHandler();
    this->timeout = timeout;
    this->retry_count = retry_count;
}

chat_client::UdpCommunicator::~UdpCommunicator() {
    delete this->udp;
    delete this->packetHandler;
}

void chat_client::UdpCommunicator::write_to_chat_win(WINDOW* win,std::string who,char* message) {
        
        int y,x;
        std::time_t result = std::time(nullptr);
        
        getyx(win,y,x);
        std::stringstream stream;
        stream << "["<< who <<"] " << message <<"\t" << std::asctime(std::localtime(&result))<<  std::endl;
        mvwprintw(win,y,x+3,stream.str().c_str());
        wrefresh(win);
        

}

void cleanningup_window_udp() {
	endwin();
}

void chat_client::UdpCommunicator::write_data_to_server(WINDOW* msg_win,WINDOW* chat_win) {
    while(true) {
        char* buffer = (char*)malloc(sizeof(char)*1024);

        wgetstr(msg_win,buffer);
        werase(msg_win);

        mtx.lock();
        write_to_chat_win(chat_win,"me",buffer);
        mtx.unlock();

        mtx.lock();
        int msg_y,msg_x;
        getyx(msg_win,msg_y,msg_x);
        mvwprintw(msg_win,msg_y+1,msg_x+3,"Write something: ");
        wrefresh(msg_win);
        mtx.unlock();

        struct chat_message_info parsed_msg = parse_message(buffer);

        std::string auth("/AUTH");
        std::string join("/JOIN");
        std::string rename("/RENAME");
        std::string bye("/BYE");


        if (parsed_msg.command == auth) {
            this->display_name = parsed_msg.display_name;
            udp->net_send(this->packetHandler->auth_udp(client_message_id,(char*)parsed_msg.username.c_str(),(char*)parsed_msg.display_name.c_str(),(char*)parsed_msg.secret.c_str()));
            this->auth_msg_sent = true;
            this->auth_done = true;

        } else if (parsed_msg.command == join) {
            if (auth_done) {
                udp->net_send(this->packetHandler->join_udp(client_message_id,(char*)parsed_msg.channel_id.c_str(),(char*)this->display_name.c_str()));
                client_message_id++;// changeable.
            } else {
                write_to_chat_win(chat_win,"ERR"," : You need to be authenticated to join in a channel.");
            }

        } else if (parsed_msg.command == rename) {
            if (auth_done) {
                this->display_name = parsed_msg.display_name;
                char* name = (char*) malloc(sizeof(char)*1024);
                strcat(name,"Name changed to ");
                strcat(name,display_name.c_str());

                write_to_chat_win(chat_win,"me", name);
            } else {
                write_to_chat_win(chat_win,"ERR"," : You need to be authenticated to rename username.");
            }

        } else if (parsed_msg.command == bye) {
            udp->net_send(this->packetHandler->bye_udp(client_message_id));
            cleanningup_window_udp();
            exit(0);

        } else {
            if (auth_done) {
                udp->net_send(this->packetHandler->msg_udp(client_message_id,(char*)this->display_name.c_str(),buffer));
                client_message_id++;
            } else {
                write_to_chat_win(chat_win,"ERR"," : The command that you have used is invalid.");
            }
        }
    }
}

void chat_client::UdpCommunicator::read_data_from_server(WINDOW* msg_win,WINDOW* chat_win) {
    std::time_t result = std::time(nullptr);
        uint8_t* buf = (uint8_t*)malloc(sizeof(uint8_t)*1024);

        while(1){
            uint8_t* buf = (uint8_t*)malloc(sizeof(uint8_t)*1024);
            buf = this->udp->net_read();

            struct UdpData extractedData = this->packetHandler->read_udp_msg(buf);

            if (extractedData.msg_type == MessageType::TYPE_REPLY) {
                if (std::find(server_message_list.begin(), server_message_list.end(),extractedData.msg_id)!=server_message_list.end()) {
                    this->udp->net_send(this->packetHandler->confirm_udp(extractedData.msg_id));
                } else {
                    write_to_chat_win(chat_win,"server", extractedData.msg_content);
                    server_message_list.push_back(extractedData.msg_id);

                    this->udp->net_send(this->packetHandler->confirm_udp(extractedData.msg_id));
                }
                
            } else if (extractedData.msg_type == MessageType::TYPE_CONFIRM)  {
                if (auth_msg_sent) {
                    if (auth_confirm_received == false) {
                        auth_confirm_received = true;
                        this->udp->change_to_dyn_port_sock();

                        if (client_message_id == extractedData.ref_msg_id) {
                            client_message_id++;
                        }
                    }
                    if (client_message_id == extractedData.ref_msg_id) {
                        client_message_id++;
                    }
                }

            } else if (extractedData.msg_type == MessageType::TYPE_MSG) {
                write_to_chat_win(chat_win,extractedData.display_name,extractedData.msg_content);
                server_message_list.push_back(extractedData.msg_id);
                
                this->udp->net_send(this->packetHandler->confirm_udp(extractedData.msg_id));

            } else if (extractedData.msg_type == MessageType::TYPE_BYE) {
                this->udp->net_send(this->packetHandler->confirm_udp(extractedData.msg_id));
                server_message_list.push_back(extractedData.msg_id);

            } else if (extractedData.msg_type == MessageType::TYPE_ERR) {
                spdlog::info("Error detected");
                for (int i =0 ; i< 100; i++) {
                    printf("%02x",buf[i]);
                }
                write_to_chat_win(chat_win,extractedData.display_name,extractedData.msg_content);

                write_to_chat_win(chat_win,"CLI","Press any key to exit...");
                getch();
                cleanningup_window_udp();
                exit(0);

            }
            else {
                write_to_chat_win(chat_win,"server","test");
                
                this->udp->net_send(this->packetHandler->confirm_udp(0));
            }
    }
}

