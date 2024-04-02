#pragma once

#include <iostream>
#include "NetWork.hpp"
#include "Communicator.hpp"
#include <chat_client/PacketHandler.hpp>
#include <ncurses.h>
#include <mutex>
#include "helper.hpp"
#include "Util.hpp"

namespace chat_client {
    /**
     * <h1>UdpCommunicator</h1>
     * A class that implements high level network communication using udp protocol(NetWork::Udp).
    */
    class UdpCommunicator : public chat_client::Communicator{
        private:
            chat_client::NetWork::Udp *udp;
            chat_client::PacketHandler *packetHandler;
            std::string username;
            std::string display_name;
            std::mutex mtx;
            bool auth_msg_sent = false;
            bool auth_confirm_received = false;
            bool auth_done = false;
            int client_message_id = 0;
            std::vector<int> server_message_list;
            int retry_count;
            int timeout;
            
        public:
            UdpCommunicator(std::string host,int port,int retry_count, int timeout);

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
            

            ~UdpCommunicator();
    };
}