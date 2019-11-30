#ifndef SERVER
#define SERVER
#include <unistd.h> 
#include <cstdio> 
#include <cstdlib> 
#include <cstring> 
#include <sys/socket.h> 
#include <netinet/in.h> 

namespace RemoteShell {

    class Server{ 
    private:
        int m_server_fd; // socket file descriptor
        int m_port_id;
        struct sockaddr_in m_address; 
        char m_buffer[1024] = {0}; 
    public:
        Server();
        Server(int port);
        ~Server();
        int setup();
        int welcome();
    };
}
#endif
