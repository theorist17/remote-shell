#ifndef _CLIENT
#define _CLIENT
#include <cstdio> 
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <cstring> 
#include <fcntl.h>
#include <sys/stat.h>
#include <csignal>

using namespace std;

static void on_intr(int s)
{
    fprintf(stdout, "No Ctrl-C, type exit to quit\n");
}

namespace RemoteShell {

	class Client {
	private:
		char m_ip_id[18];
		int m_port_id;
		struct sockaddr_in m_serv_addr;
		char m_incoming[1024];
		char m_outgoing[1024];
		int m_sock;
	public:
		Client();
		Client(char* ip, int port);
		~Client();
		int connect();
		char* run();
        string download(const char* path);
        string upload(const char* path);
	};
}
#endif
