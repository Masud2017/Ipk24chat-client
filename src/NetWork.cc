#include <chat_client/NetWork.hpp>


chat_client::NetWork::NetWork() {

	spdlog::info("Hello world trying to do some hard working stuff");
}

chat_client::NetWork::Tcp::Tcp(char* host,int port) {
	struct timeval timeout = {.tv_sec = CONFIRMATION_TIMEOUT, .tv_usec = 0};
	int yes = 1;
	
	sock_fd = socket(AF_INET,SOCK_STREAM,0); // getting the socket 
	if (sock_fd == 0) {
		printf("Socket failed\n");
	}


	if (setsockopt(sock_fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
		perror("setsockopt failed");
		// Handle error
	}


	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	if (inet_pton(AF_INET, host, &address.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
    }
 
	std::cout << "Printing the value of sock fd : "<<sock_fd<<std::endl;


	printf("\nConnecting to port : %d - and host : %s",port,host);
	

	int status;

	if ((connect(sock_fd, (struct sockaddr*)&address,
                   sizeof(address)))
        < 0) {
        printf("\nConnection Failed \n");
    }
	
}


chat_client::NetWork::Tcp::Tcp(std::string remote_server_ip, int port) {
	server_port = port;
	server_host = remote_server_ip;

	sock_fd = socket(AF_INET,SOCK_STREAM,0); // getting the socket 
	if (sock_fd == 0) {
		printf("Socket failed\n");
	}
}

chat_client::NetWork::Tcp::~Tcp() {
	spdlog::info("Cleaning up the Network::Tcp object, closing socket file descriptor");
	close(sock_fd);
}


std::string chat_client::NetWork::Tcp::net_read() {
	char *buffer = (char*)malloc(sizeof(char) *1024);
	recv(sock_fd,buffer,1024,0);
	
	std::string str(buffer);
	
	return str;
}

void chat_client::NetWork::Tcp::net_send(char* sendableData) {
	send(sock_fd,sendableData,strlen(sendableData),0);
}



chat_client::NetWork::Udp::Udp(char* host,int port) {
	struct timeval timeout;      
    timeout.tv_sec = CONFIRMATION_TIMEOUT;
    timeout.tv_usec = 0;

	bzero(&address, sizeof(address)); 
    address.sin_addr.s_addr = inet_addr(host); 
    address.sin_port = htons(port); 
    address.sin_family = AF_INET; 
      
	 

	sock_fd_udp = socket(AF_INET, SOCK_DGRAM, 0); // getting the socket 
	if (sock_fd_udp == 0) {
		printf("Socket failed\n");
	}

	// setting up timeout to the socket file descriptor
	if (setsockopt (sock_fd_udp, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                sizeof timeout) < 0) {
        perror("setsockopt failed\n");
	}


	if ((connect(sock_fd_udp, (struct sockaddr*)&address,
                   sizeof(address)))
        < 0) {
        printf("\nConnection Failed \n");
		exit(0);
	}
}

uint16_t get_udp_dest_port(uint8_t *packet) {
    // Assuming packet points to the beginning of the UDP header
    struct udphdr *udp_header = (struct udphdr *)packet;
    return ntohs(udp_header->dest); // Convert network byte order to host byte order
}
uint8_t* chat_client::NetWork::Udp::net_read() {
	uint8_t* buffer = (uint8_t*) malloc(sizeof(uint8_t) * 100);
	socklen_t len = sizeof(address);
	
	for (int i = 0; i < RETRY_COUNT; i++) {
		int read_count;
		read_count = recvfrom(sock_fd_udp, buffer, 100, 0, (struct sockaddr*)&address,&len); 
		if (read_count > 0) {
			break;
		}
	}

	if (dyn_port_required) {
		
		if(connect(sock_fd_udp, (struct sockaddr *)&address, sizeof(address)) < 0) 
		{ 
			printf("\n Error : Connect Failed \n"); 
			exit(0); 
	}

	}	
	return buffer;

}


void chat_client::NetWork::Udp::net_send(uint8_t* buffer) {
	for (int i = 0; i < RETRY_COUNT; i++) {
		int send = 0;
		send = sendto(sock_fd_udp, buffer, 1024, 0, (struct sockaddr*)NULL, sizeof(address)); 
		if (send > 0) {
			break;
		}
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			perror("sendto failed");
		}
	}
}


void chat_client::NetWork::Udp::change_to_dyn_port_sock() {
	address.sin_port = 0; // port wildcard

	 if(connect(sock_fd_udp, (struct sockaddr *)&address, sizeof(address)) < 0) 
    { 
        printf("\n Error : Connect Failed \n"); 
        exit(0); 
    }
	dyn_port_required = true;

	
}