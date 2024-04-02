#pragma once

#include <iostream>
#include <ncurses.h>
#include <thread>


namespace chat_client {  
    /**
     * <h1>Communicator</h1>
     * It's an abstract class that has the blueprint for all kinds of network communicators such as TcpCommunicator and UdpCommunicator.
    */
    class Communicator {
        public:
            
            virtual void write_data_to_server(WINDOW* msg_win,WINDOW* chat_win) = 0;
            virtual void read_data_from_server(WINDOW* msg_win,WINDOW* chat_win) = 0;

            void write_to_chat_win(WINDOW* win,std::string who,char* message);
    };
} 
