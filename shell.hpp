#ifndef _SHELL
#define _SHELL
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h> 
#include <sys/socket.h>

namespace RemoteShell{
    class Shell{ private:
        int m_sockfd;
        char m_incoming[1024];
        char m_outgoing[1024];
    public:
        Shell();
        Shell(int sockfd);
        ~Shell();
        int run();
    };
}
#endif
