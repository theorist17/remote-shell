#ifndef SERVER
#define SERVER
#include <cstdio> 
#include <cstdlib> 
#include <cstring> 
#include <unistd.h> 
#include <iostream>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <pthread.h> //for threading , link with lpthread
#include "shell.hpp"
#include <string>
#include <csignal>


namespace RemoteShell {

	class Server {
	private:
		int m_server_fd; // socket file descriptor
		int m_port_id;
		struct sockaddr_in m_address;
		char m_buffer[1024] = { 0 };
	public:
		Server();
		Server(int port);
		~Server();
		int setup();
		static void* handler(void* sockfd);
		int run();
	    vector<pthread_t> m_threads;
	};
}
#endif
