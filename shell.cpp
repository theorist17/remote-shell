#include "shell.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>

using namespace std;
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

        memset(m_outgoing, 0, 1024);
        if (strcmp(m_incoming, "exit")==0)
        {
            strcpy(m_outgoing, m_incoming);
        }
        else
        {
            char buffer[1024];
            std::string result = "";
            FILE* pipe = popen(m_incoming, "r");
            if (!pipe) throw std::runtime_error("popen() failed!");
            try {
                result += " ";
                while (!feof(pipe)) {
                    if (fgets(buffer, 1024, pipe) != NULL)
                        result += buffer;
                }
            } 
            catch (...) {
                pclose(pipe);
                result = "Not Found";
                throw;
            }
            pclose(pipe);
            strcpy(m_outgoing, result.c_str());
        }

        send(m_sockfd, m_outgoing, strlen(m_outgoing) , 0 ); 
        if (strcmp(m_incoming, "exit")==0)
        {
            printf("Shell terminated.\n");
            break;
        }
    }
    return 0;
}
