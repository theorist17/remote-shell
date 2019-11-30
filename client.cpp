#include "client.hpp"
using namespace RemoteShell;

Client::Client()
{
    strcpy(m_ip_id, "192.168.35.117");
    m_port_id = 8080;

    memset(m_buffer, 0, 1024);
    connect();
}

Client::Client(char* ip, int port)
{
    strcpy(m_ip_id, ip);
    m_port_id = port;

    memset(m_buffer, 0, 1024);
    connect();
}

Client::~Client()
{
}

int Client::connect() 
{
  	if ((m_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  	{ 
  		printf("\n Socket creation error \n"); 
  		return -1; 
  	} 

  	m_serv_addr.sin_family = AF_INET; 
  	m_serv_addr.sin_port = htons(m_port_id); 
  	
  	// Convert IPv4 and IPv6 addresses from text to binary form 
  	if(inet_pton(AF_INET, m_ip_id , &m_serv_addr.sin_addr)<=0) 
  	{ 
  		printf("\nInvalid address/ Address not supported \n"); 
  		return -1; 
  	} 

  	if (::connect(m_sock, (struct sockaddr *)&m_serv_addr, sizeof(m_serv_addr)) < 0) 
  	{ 
  		printf("\nConnection Failed \n"); 
  		return -1; 
    }

    hello();

  	return 0; 
} 

char* Client::hello()
{
    int valread;
    char* p_buffer = m_buffer;
    while(true) {
        memset(m_buffer, 0, 1024);
        scanf("%s", p_buffer);
        send(m_sock , m_buffer, strlen(m_buffer) , 0 ); 
        memset(m_buffer, 0, 1024);
        valread = recv( m_sock , m_buffer, 1024, 0); 
        printf("%s\n", m_buffer);
        if (strcmp(m_buffer, "exit")==0)
        {
            break;
        }
            
    }
    return NULL;
}


int main(int argc, char** argv)
{
    char dummy[100] = "192.168.35.117";
    Client client(dummy, 8080);
    return 0;
}
