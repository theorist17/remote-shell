#include "shell.hpp"
using namespace RemoteShell;

Shell::Shell()
{
}

Shell::Shell(int sockfd)
{
    m_sockfd = sockfd;
    run();
}

Shell::~Shell()
{
}

int Shell::run(){
    int valread;
    printf("Shell started.\n");
    while(true)
    {
        memset(m_buffer, 0, 1024);
        valread = read( m_sockfd, m_buffer, 1024); 
        m_buffer[valread] = '\0'; // marking last message NULL 

        send(m_sockfd, m_buffer, strlen(m_buffer) , 0 ); 
        if (strcmp(m_buffer, "exit")==0)
        {
            printf("Shell terminated.\n");
            break;
        }
    }
    return 0;
}
