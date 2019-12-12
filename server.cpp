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
    for (int i = 0 ; i < m_threads.size(); i++)
    {
        pthread_kill(m_threads[i], SIGKILL);
    }
	printf("Closing server..\n");
}

int Server::setup()
{
	int opt;

	// Creating socket file descriptor 
	if ((m_server_fd = socket(PF_INET, SOCK_STREAM, 0)) == 0)
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
	m_address.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1");
	m_address.sin_port = htons(m_port_id);
	memset(m_address.sin_zero, '\0', sizeof m_address.sin_zero); //Set all bits of the padding field to 0


																 // Attching socket on to the specified port
	if (bind(m_server_fd, (struct sockaddr *)&m_address, sizeof(m_address))<0)
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

	run();

	return 0;
}

int Server::run() {
	int addrlen = sizeof(m_address);
	int new_socket;
	size_t cid;
    

	while (true)
	{
		if ((new_socket = accept(m_server_fd, (struct sockaddr *)&m_address, (socklen_t*)&addrlen)) < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}

        pthread_t thread_id;
        m_threads.push_back(thread_id);
		if (pthread_create(&thread_id, NULL, handler, (void*)&new_socket) < 0)
		{
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

void* Server::handler(void* socket_desc)
{
	int sock = *(int*)socket_desc;
	Shell shell(sock);

	return 0;
};


int main(int argc, char** argv) {
	Server server(8080);

	return 0;
}

