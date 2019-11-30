#include "server.hpp"
using namespace RemoteShell;

Server::Server()
{
    int portnum;
    printf("Please enter port number to use.\n");
    scanf("%d", &m_port_id);
    setup();
}

Server::Server(int portnum)
{
    m_port_id = portnum;
    setup();
}

Server::~Server()
{
    printf("Closing server..\n");
}

int Server::setup() 
{ 
    int opt;

    // Creating socket file descriptor 
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

    // Configuring socket options such as API level
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 

    m_address.sin_family = AF_INET; 
    m_address.sin_addr.s_addr = INADDR_ANY; 
    m_address.sin_port = htons( m_port_id ); 

    // Attching socket on to the specified port
    if (bind(m_server_fd, (struct sockaddr *)&m_address,  
                                 sizeof(m_address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Start litenning
    if (listen(m_server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 

    welcome();
    return 0; 
}

int Server::welcome(){
    int addrlen = sizeof(m_address); 
    int new_socket;
    int valread;
    size_t cid;
    char hello[100] = "Hello. ";

    while( true ) 
    { 
        printf("Waiting for connection...\n");

        if ( (new_socket = accept(m_server_fd, (struct sockaddr *)&m_address,  (socklen_t*)&addrlen)) < 0)
        {
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        
        valread = read( new_socket , m_buffer, 1024); 
        printf("Message from client : %s\n", m_buffer ); 
        send(new_socket , hello , strlen(hello) , 0 ); 
        printf("Hello message sent\n"); 
        valread = read( new_socket , m_buffer, 1024); 
        printf("Message from client : %s\n", m_buffer ); 
        send(new_socket , hello , strlen(hello) , 0 ); 
        printf("Hello message sent\n"); 

        //cid = fork();
        //if (!cid) {
        //  char client_socket_buf[6];
        //  sprintf(client_socket_buf, "%d", new_socket);
        //  const char *p = client_socket_buf;
        //  execl("./RemoteShell", p, NULL);
        //}
    } 

    return 0;
}

