#ifndef _SHELL
#define _SHELL
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h> 
#include <sys/socket.h>
#include <iostream>
using namespace std;

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
        string exec(const char* cmd);
    };
}
#endif
