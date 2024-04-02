#pragma once

#include "Util.hpp"
#include "Communicator.hpp"
#include "helper.hpp"
#include "NetWork.hpp"

#include <iostream>
#include <chat_client/PacketHandler.hpp>
#include <ncurses.h>
#include <mutex>

#include <string.h>
#include <sstream>
#include <ctime>


namespace chat_client {
    /**
     * <h1>TcpCommunicator</h1>
     * A class that uses NetWork::Tcp class and wrap that class to implement high level network operations over tcp protocol.
    */
    class TcpCommunicator : public chat_client::Communicator {
        private:
            chat_client::PacketHandler *packetHandler;
            chat_client::NetWork::Tcp *tcp;
            std::mutex mtx;
            std::string username;
            std::string display_name;
            bool auth_done = false;
        

        public:
            TcpCommunicator(std::string host,int port);

            /**
             * It is responsible to write output to the chat screen.
            */
            void write_to_chat_win(WINDOW* win,std::string who,char* message);

            /**
             * This method runs in a separate thread independent from the main thread.
             * It is responsible to write output to the chat screen.
            */
            void write_data_to_server(WINDOW* msg_win,WINDOW* chat_win);
            /**
             * This method runs in a separate thread independent from the main thread.
             * It is responsible to read data from server and then print that to the chat window
            */
            void read_data_from_server(WINDOW* msg_win,WINDOW* chat_win);



            
            

            ~TcpCommunicator();
    };
}