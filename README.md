# IPK Project 1
Client for a chat server using IPK24-CHAT protocol

- [Installation guide](#prerequisities)
- [Executive summary of used protocols](#protocols)
- [Implementation](#implementation)
- [Network Handler Class](#network-handler-class)
- [TCP communication](#tcp-communication)
- [TCP Class](#tcp-class)
- [UDP Communicator](#udp-communication)
- [Tests](#tests)
- [TCP Tests](#tcp-tests)
- [UDP Tests](#udp-tests)
- [Extra features](#extra-features)
- [References](#references)

### Prerequisities
[Ncurses](https://www.cyberciti.biz/faq/linux-install-ncurses-library-headers-on-debian-ubuntu-centos-fedora/) library is required. It can be installed by using following command:
```
sudo apt-get install libncurses5-dev libncursesw5-dev
```

### Get remaining libraries spdlog and argparse with following commands:
```
git submodule init
git submodule update
```

### Build ipk24chat-client:
```
cmake -S . -B .
make -j
```

### Execute with following command.
```
./ipk24chat-client -t [protocol] -p [port] -s [host]
```

#### You can learn more about the program with following command.
```
./ipk24chat-client --help/-?
```




## Protocols

### Transmission control protocol - TCP
Transmission control protocol operates atop internet protocol to guarantee the dependable delivery of data packets across networks, including the vast realm of the internet. Distinguished as a connection-oriented protocol, TCP diligently forges and preserves a link between communicating entities until data exchange reaches its conclusion. Addressing challenges inherent in packet-based communication, TCP adeptly navigates lost packets, packet disarray, duplications, and data corruption. This feat is accomplished through an arsenal of techniques including sequence and acknowledgment numbering, checksums, flow regulation, error rectification, and congestion management.

### User datagram protocol - UDP
User datagram protocol offers a streamlined and effective means to dispatch and collect datagrams across an IP network. While UDP doesn't assure the delivery sequence, or accuracy of data, it significantly reduces the burden and delay associated with data transmission compared to TCP. Ideal for scenarios prioritizing speed over everything else.



## Implementation
This program is written using Object oriented pattern. Object orientated patter Single responsibility is maintained for every class.

![Something went wrong](class%20%20diag.png)


The program begins by parsing command line arguments using argparse library.

``` c++
struct MyArgs : public argparse::Args {
        std::string &t = kwarg("t,proto", "Protocol type either tcp or udp");
        std::string &s = kwarg("s,host", "Host address for remote server");
        int &p = kwarg("p,port", "Port to connect to the server");
        int &d = kwarg("d,timeout", "Udp confirmation timeout").set_default(140);
        int &r = kwarg("r,retry", "Maximum number of UDP retransmissions").set_default(3); // retry count

        bool &verbose = flag("v,verbose", "A flag to toggle verbose");
    };


    auto args = argparse::parse<MyArgs>(argc, argv);
    if (args.verbose) {
        args.print();
    }

    std::string host = args.s;
    std::string host_ip = std::string(get_ip_addr_from_host((char*)host.c_str(),(char*)std::to_string(args.p).c_str()));

    if (args.t == "tcp") {
        this->communicator = new chat_client::TcpCommunicator(host_ip,args.p);
    } else if (args.t == "udp") {
        std::cout << "Your entered protocol type is udp" <<std::endl;
        this->communicator = new chat_client::UdpCommunicator(host_ip, args.p, args.r, args.d);
    } else {
        std::cout << "Your entered protocol type is invalid" <<std::endl;
        exit(1);
    }
```

### Network class including NetWork::Tcp and NetWork::Udp

The class is impelementing lowlevel socket connection between client and server. This class will be used by other class to more layer to it.

``` c++
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
```

### TCP communication
To communicating in tcp mode the client has to use the "TcpCommunicator" class. This class uses "NetWork::Tcp" and "PacketHandler" classes and implements high level communication with the server using ipk24chat protocol.

#### TCP Class

The class implements the following methods, similar structure is used in the [UdpCommunicator](#udp-communication).

- `TcpCommunicator(std::string host,int port)`: A public construct for TcpCommunicator that takes two arguments host and port. It initializes connection between server and client.

    ``` c++
    this->communicator = new chat_client::TcpCommunicator(host_ip,args.p);
    ```

- `void write_data_to_server(WINDOW* msg_win,WINDOW* chat_win)`: A public method reads user input and send that to the server.

    ``` c++
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
    ```

- `void read_data_from_server(WINDOW* msg_win,WINDOW* chat_win)`: A public method that reads response from server and print that to the chat window.

    ``` c++
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
    ```

#### Udp Communication

The class does the same as the TcpCommunicator does except the protocol is different, it is using UDP protocol. Since the udp protocol stateless and has issues with data intigrity, we need to use some tactics to make sure that both the client and server receiving and sending data successfully.

- `UdpCommunicator(std::string host,int port,int retry_count, int timeout)`: A public construct for UdpCommunicator that takes two arguments host and port. It initializes connection between server and client.

    ``` c++
    this->communicator = new chat_client::UdpCommunicator(host_ip, args.p, args.r, args.d);
    ```

- `void write_data_to_server(WINDOW* msg_win,WINDOW* chat_win)`: Same as the tcp variant, It takes user input and send that to the server and also prints thta to the chat window.

    ``` c++
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
    ```

- `void read_data_from_server(WINDOW* msg_win,WINDOW* chat_win)`: It receives data from server and prints to the chat window.

    ``` c++
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
    ```

## Tests

### TCP Tests

- Authentication

 1. `AUTH xdolez97 AS user USING secret\r\n`
 2. `REPLY {"OK"} IS Authentication successfull\r\n`

- Incorect command

 1. `/wrong_command`
 2. Error

- Simple exchange

 1. `MSG FROM DisplayName IS MessageContent\r\n`
 2. Message Reply if any user exists
 3. `BYE`
 4. BYE

- Multiple exchanges

 1. `MSG FROM user IS MessageContent user\r\n`
 2. Reply from to user from random user
 3. `MSG FROM lina IS MessageContent lina\r\n`
 4. Reply from to lina from random user
 5. `BYE`
 6. BYE

- Incorrect exhange

 1. `MSG FROM user IS MessageContent user\r\n`
 2. BYE


### UDP Tests

- Simple exchange

 1. `|2|0|username|0|User_1|0|Secret|0|`
 2. |0|0| {CONFIRM}

- Multiple exchanges

 1. `|2|0|username|0|User_1|0|Secret|0|` auth request from client.
 2. `|0|0| {CONFIRM}` confirmation from server
 3. `|1|0|1|OK|` REPLY from server
 4. `|0|0| {CONFIRM}` confirmation from client
 5. `|4|1|Server|0|Joined default.|0` message from server
 6. `|0|1| {CONFIRM}` confirmation from client
 7. `|255|1|` Bye from client
 8. `|0|1|` confirm from server

- Incorrect exhange

 1. `|10|0|username|0|User_1|0|Secret|0|`
 2. ERR:Could not parse the message
 3. `|2|0|username|0|User_1|0|Invalid_secret|0|`
 4. ERR


## References

Barkley Socket api wikipedia, <https://en.wikipedia.org/wiki/Berkeley_sockets>

Initial connection protocol, <https://datatracker.ietf.org/doc/html/rfc1350#autoid-4>

Network byte order ibm, <https://www.ibm.com/docs/en/zos/2.4.0?topic=hosts-network-byte-order>
