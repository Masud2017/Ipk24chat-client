#include <chat_client/Application.hpp>


chat_client::Application::Application(int argc, char** argv) {
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

}




void chat_client::Application::start_threads() {
    std::thread write_thread([this]() { this->communicator->write_data_to_server(this->msg_win,this->chat_win); });
    std::thread read_thread([this]() { this->communicator->read_data_from_server(this->msg_win,this->chat_win); });

    write_thread.join();
    read_thread.join();

}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	// box(local_win, 0 , 0);		/* 0, 0 gives default characters 
	// 				 * for the vertical and horizontal
	// 				 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}
void destroy_win(WINDOW *local_win) {	
	// wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	// wrefresh(local_win);
	delwin(local_win);
}


void signal_end(int signum) {
	endwin();
}


void chat_client::Application::split_screen_horizontal() {
    WINDOW* chat_window;
    WINDOW* msg_window;

    int x, y;
    int chat_x,chat_y;
    
    initscr();			/* Start curses mode 		*/
    getyx(stdscr,y,x);
    
	
    printw("hello world");
    refresh();
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // trying to get the terminal window size
    this->chat_win = create_newwin(w.ws_row - 4,w.ws_col,x,y);

    scrollok(chat_win,true);
    if (has_colors() == TRUE) {
        printw("Color exists");
        start_color();			/* Start color 			*/
        init_color(COLOR_RED,16,191,224);
        init_pair(1, COLOR_WHITE, COLOR_RED);
        wbkgd(chat_win,COLOR_PAIR(1));
        // wattron(chat_win,COLOR_PAIR(1);
    }

    // mvwprintw(chat_win,y+1,x+3,"Hello world\n");
    wrefresh(chat_win);
    getmaxyx(chat_win,chat_y,chat_x);

    this->msg_win = create_newwin(4,w.ws_col,chat_y,x);

    if (has_colors() == TRUE) {
        printw("Color exists");
        start_color();			/* Start color 			*/
        init_color(COLOR_BLACK,224,96,34);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        wbkgd(msg_win,COLOR_PAIR(2));
        // wattron(chat_win,COLOR_PAIR(1);
    }

    int msg_y,msg_x;
    getyx(msg_win,msg_y,msg_x);
    mvwprintw(msg_win,msg_y+1,msg_x+3,"Write something: ");
    wrefresh(msg_win);
   

    this->start_threads();


    signal(SIGINT,signal_end);
    pause();
}


void chat_client::Application::start_application() {
    this->split_screen_horizontal();
}
