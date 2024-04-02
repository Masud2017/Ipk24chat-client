#pragma once

#include "PacketHandler.hpp"

#include <spdlog/spdlog.h>
#include <iostream>

#include <sys/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include <sstream>
#include <netinet/tcp.h>
#include <netinet/udp.h> // for struct udphdr



// #define DEFAULT_PORT 4567
// #define DEFAULT_HOST "147.229.8.244"
// #define DEFAULT_HOST "127.0.0.1"


#define RETRY_COUNT 3
#define CONFIRMATION_TIMEOUT 250 // ms

namespace chat_client {
	/**
	 * <h1>NetWork</h1>
	 * A parent class that holds Tcp and Udp Classes
	*/
	class NetWork {
		public:
			/**
	 		* <h1>Tcp</h1>
			* This class implements all the types of low level tcp network socket connections.
			*/
			class Tcp {
				private:
					int server_port;
					std::string server_host;
					struct sockaddr_in address;
					
					

				public:
					int sock_fd;
					Tcp(char* host,int port); // Constructor
					Tcp(std::string remote_server_ip,int port);
					~Tcp(); // destructor
					
					/**
					 * @deprecated
					 * Using helper(RIO) instead
					*/
					std::string net_read();
					/**
					 * sends data via tcp network
					*/
					void net_send(char* sendableData);
			};

			/**
	 		* <h1>Udp</h1>
			* This class implements all the types of low level udp network socket connections.
			*/
			class Udp {
				private:
					int server_port;
					int sock_fd_udp;
					struct sockaddr_in address;
					bool dyn_port_required = false;
					bool dyn_port_requirement_done = false;
				public:
					Udp(char* host,int port);

					/**
					 * Reads data that coming from udp connection
					*/
					uint8_t *net_read();
					/**
					 * changes default port to dynamic port 0, so that the system can choose the open port the for the program.
					*/
					void change_to_dyn_port_sock();
					/**
					 * send data to the server in udp connection
					*/
					void net_send(uint8_t* buffer);
			};
			NetWork(); // contstructor
			// ~NetWork(); // destructor
	};
}
