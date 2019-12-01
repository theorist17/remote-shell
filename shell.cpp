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
        memset(m_incoming, 0, 1024);
        valread = read( m_sockfd, m_incoming, 1024); 
        m_incoming[valread] = '\0'; // marking last message NULL 

        // 여기에 쉘의 명령이 들어갑니다.
        strcpy(m_outgoing, m_incoming);

        send(m_sockfd, m_outgoing, strlen(m_outgoing) , 0 ); 
        if (strcmp(m_incoming, "exit")==0)
        {
            printf("Shell terminated.\n");
            break;
        }
    }
    return 0;
}
