#include "client.hpp"
using namespace RemoteShell;


Client::Client()
{
    printf("IP address : ");
    char ip[18];
    scanf("%s", ip);
    strcpy(m_ip_id, ip);
    printf("Port number : ");
    scanf("%d", &m_port_id);

	connect();
}

Client::Client(const char* ip, int port)
{
	strcpy(m_ip_id, ip);
	m_port_id = port;

	connect();
}

Client::~Client()
{
    char bye[1024]="exit";
    send(m_sock, bye, strlen(bye), 0);
    char confirm[1024];
    memset(confirm, 0, 1024);
	recv(m_sock, confirm, 1024, 0);
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

    struct sigaction act;
    act.sa_handler = on_intr;
    act.sa_flags = 0;
    //if((sigemptyset(&act.sa_mask)==-1)||
    //            (sigaction(SIGINT, &act, NULL) == -1))
    //    perror("Failed to install SIGINT signal handler");

	run();

	return 0;
}

char* Client::run()
{
	int valread;
	char* p_buffer = m_outgoing;
	string str;
	while (true) {
		// Read user input here.
		// Don't send more than any sentence or file which has length of more than 1024 bytes
		memset(m_outgoing, 0, 1024);
		printf("RemoteShell ▶ ");
		getline(cin, str);

		send(m_sock, str.c_str(), str.size(), 0);

        if (!string(str).substr(0,6).compare("upload"))
        {
            string file = string(str).substr(7), target;
            if(target = upload(file.c_str()), target.length()!=0)
                cout << "Uploaded " << file << " as " << target << endl;
            else 
                cout << "Upload failed" << endl;
            continue;
        }

		memset(m_incoming, 0, 1024);
		valread = recv(m_sock, m_incoming, 1024, 0);

        if (!string(str).substr(0,8).compare("download"))
        {
            string file = string(str).substr(9), target;
            if(target = download(file.c_str()), target.length()!=0)
                cout << "Downloaded " << file << " as " << target << endl;
            else 
                cout << "Download failed" << endl;
            continue;
        }

		printf("==============================\n\n");
		printf("%s", m_incoming);
		printf("\n==============================\n");


		if (strcmp(m_incoming, "exit") == 0) // 클라이언트가 exit을 보내면
											 // 서버가 exit을 받고 되돌려 보내면 종료
		{
			break;
		}
	}
	return NULL;
}

// upload passed file, get server's response, return the target file name if succeded, else an empty string
string Client::upload(const char* filename)
{
    int fd, bytesread, size = 1024;
    char file_buf[1024], msg_buf[1024];
    memset(file_buf, 0, 1024);
    memset(msg_buf, 0, 1024);
    while( fd = open(filename, O_RDONLY), fd == -1 && errno == EINTR);
    while( bytesread = read(fd, file_buf, size), bytesread == -1 && errno == EINTR);
	send(m_sock, file_buf, strlen(file_buf), 0);
    close(fd);

	recv(m_sock, msg_buf, 1024, 0);
    
    string target(msg_buf);
    if(!target.compare("upload failed"))
        return string();
    else
        return target;
}

string Client::download(const char* filename)
{
    int fd;
    string target(filename);
    while(fd = open(target.c_str(), O_WRONLY|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR), fd == -1 && (errno == EINTR || errno == EEXIST))
    {
        if (errno == EEXIST){
            int ind;
            if((ind = target.find_last_of("."))==string::npos)
            {
                target = target.append("_");
            }
            else
            {
                string ext = target.substr(ind+1);
                target = target.substr(0, ind).append("_.").append(ext);
            }
        }
    }
    
    int bytesrecv = string(m_incoming).length();

    char *bufp;
    size_t bytestowrite;
    ssize_t byteswritten;
    size_t totalbytes;

    for (bufp = m_incoming, bytestowrite = bytesrecv, totalbytes = 0;
            bytestowrite > 0;
            bufp += byteswritten, bytestowrite -= byteswritten)
    {
        byteswritten = write(fd, bufp, bytestowrite);
        if(byteswritten == -1 && errno != EINTR)
            return string();
        if (byteswritten == -1)
            byteswritten = 0;
        totalbytes += byteswritten;
    }
    close(fd);
    return target;
}


int main(int argc, char** argv)
{
	Client client("127.0.0.1", 8080);
	//Client client;
	return 0;
}
