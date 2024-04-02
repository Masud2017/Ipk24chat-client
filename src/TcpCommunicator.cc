#include <chat_client/TcpCommunicator.hpp>

chat_client::TcpCommunicator::TcpCommunicator(std::string host,int port) {

    this->tcp = new chat_client::NetWork::Tcp((char*)host.c_str(),port);

    packetHandler = new chat_client::PacketHandler();

}


chat_client::TcpCommunicator::~TcpCommunicator() {
    delete this->tcp;
    delete packetHandler; // free up the resources(memory)
}


void chat_client::TcpCommunicator::write_to_chat_win(WINDOW* win,std::string who,char* message) {
        
        int y,x;
        std::time_t result = std::time(nullptr);
        
        getyx(win,y,x);
        std::stringstream stream;
        stream << "["<< who <<"] " << message <<"\t" << std::asctime(std::localtime(&result))<<  std::endl;
        mvwprintw(win,y,x+3,stream.str().c_str());
        wrefresh(win);
        

}

void cleanningup_window() {
	endwin();
}

void chat_client::TcpCommunicator::write_data_to_server(WINDOW* msg_win,WINDOW* chat_win) {
    while(true) {
        char* buffer = (char*)malloc(sizeof(char) * 4000);

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
            tcp->net_send((char*) this->packetHandler->auth_tcp(parsed_msg.username.c_str(),parsed_msg.display_name.c_str(),parsed_msg.secret.c_str()).c_str());
            auth_done = true;

        } else if (parsed_msg.command == join) {
            if (auth_done) {
                tcp->net_send((char*) this->packetHandler->join_tcp((char*)parsed_msg.channel_id.c_str(),(char*)this->display_name.c_str()).c_str());
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
            tcp->net_send((char*) this->packetHandler->bye_tcp().c_str());
            cleanningup_window();
            exit(0);

        } else {
            if (auth_done) {
                tcp->net_send((char*) this->packetHandler->msg_tcp(this->display_name.c_str(),buffer).c_str());
            } else {
                write_to_chat_win(chat_win,"ERR"," : The command that you have used is invalid.");
            }

        }

        
        
    }
}

void chat_client::TcpCommunicator::read_data_from_server(WINDOW* msg_win,WINDOW* chat_win) {
    std::time_t result = std::time(nullptr);
    char* buf = (char*)malloc(sizeof(char)*1024);
    rio_t rio;
    int status;
    // initialise rio data structure
    rio_readinitb(&rio, this->tcp->sock_fd);
                int y,x;

    while(1){
        while((status=rio_readlineb(&rio,buf,1024)) >0){
            //error
            if(status == -1)
                exit(1);
            
                buf[strlen(buf) - 1] = ' ';
                buf[strlen(buf) - 2] = ' ';

                struct TcpData extractedData = this->packetHandler->read_tcp_msg(buf);
                if (extractedData.msg_type == MessageType::TYPE_REPLY) {
                    std::string status("OK");
                    if (extractedData.status == status) {
                        write_to_chat_win(chat_win,"server",(char*)extractedData.msg_content.c_str());
                    }
                } else if (extractedData.msg_type == MessageType::TYPE_MSG) {
                        write_to_chat_win(chat_win,(char*)extractedData.display_name.c_str(),(char*)extractedData.msg_content.c_str());

                } else if (extractedData.msg_type == MessageType::TYPE_ERR) {
                    write_to_chat_win(chat_win,(char*)extractedData.display_name.c_str(),(char*)extractedData.msg_content.c_str());

                    write_to_chat_win(chat_win,"CLI","Press any key to exit...");
                    getch();
                    cleanningup_window();
                    exit(0);

                }
                else {
                    write_to_chat_win(chat_win,"server",buf);

                }

        }
        
		break;
        fflush(stdout);
    }
}
