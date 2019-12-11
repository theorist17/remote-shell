#include "client.hpp"
using namespace RemoteShell;

Client::Client()
{
	strcpy(m_ip_id, "127.0.0.1");
	m_port_id = 8080;

	connect();
}

Client::Client(char* ip, int port)
{
	strcpy(m_ip_id, ip);
	m_port_id = port;

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
	if (inet_pton(AF_INET, m_ip_id, &m_serv_addr.sin_addr) <= 0)
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
	char* p_buffer = m_outgoing;
	std::string str;
	while (true) {
		// 여기에서 사용자의 입력을 받습니다.
		// 혹은 파일을 불러와서 문장단위로 보내면 됩니다.
		// 1024 바이트 이상을 보내지 마세요.
		memset(m_outgoing, 0, 1024);
		printf("Shell ▶ ");
		getline(std::cin, str);
		send(m_sock, str.c_str(), str.size(), 0);

		memset(m_incoming, 0, 1024);
		valread = recv(m_sock, m_incoming, 1024, 0);
		printf("==============================\n\n");
		printf("%s", m_incoming);
		printf("\n==============================\n");


		if (strcmp(m_incoming, "exit") == 0) // 클라이언트가 exit을 보내면
											 // 서버가 exit을 받고 되돌려 보내면 종료
		{
			break;
			// 사용자의 Ctrl+C 시그널 잡아서 exit 보내도록 짜겠습니다.
		}

	}
	return NULL;
}


int main(int argc, char** argv)
{
	//char dummy[100] = "192.168.35.117";
	//Client client(dummy, 8080);
	Client client;
	return 0;
}