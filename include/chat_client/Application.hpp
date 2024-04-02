#pragma once

#include "Communicator.hpp"
#include "TcpCommunicator.hpp"
#include "UdpCommunicator.hpp"

#include <argparse/argparse.hpp>
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <thread>
#include <mutex>
#include "Util.hpp"
#include <stdlib.h>

namespace chat_client {
    /**
     * <h1>Application</h1>
     * This class handles tasks related to command line interface input and uses all the other low level 
     * code implementation in the source code.
     * It is the application interface that uses every components and produces results.
    */
    class Application {
        private:
            chat_client::Communicator *communicator;
            WINDOW* chat_win;
            WINDOW* msg_win;
            std::string username;
            std::string secret;

            void split_screen_horizontal();

        public:
            /**
             * Constructor for the class that initializes objects helpful for the program to run properly.
            */
            Application(int argc, char** argv);

            /**
             * internal method that runs two different threads. One that receives user input from the console and Second
             * one shows output to the console.
            */
            void start_threads();
            /**
             * It is mandatory to invoke this method to get everything started.
            */
            void start_application();
    };
}