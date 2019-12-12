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

int Shell::run() {
	int valread;
	printf("Shell started.\n");
	while (true)
	{
		memset(m_incoming, 0, 1024);
		valread = read(m_sockfd, m_incoming, 1024);
		m_incoming[valread] = '\0'; // marking last message NULL 

		memset(m_outgoing, 0, 1024);
		if (strcmp(m_incoming, "exit") == 0)
		{
			strcpy(m_outgoing, m_incoming);
		}
		else
		{
			std::string result = "";
			FILE* pipe = popen(m_incoming, "r");
			//printf("%s\n", m_incoming);
			if (!pipe) throw std::runtime_error("popen() failed!");
			try {
				result += " ";
				while (!feof(pipe)) {
					if (fgets(buffer, 1024, pipe) != NULL) {
						result += buffer;
					}
				}
				//printf("%s", buffer);
				//printf("%s\n", m_incoming);
				if (m_incoming[0] == 'c' && m_incoming[1] == 'd') {
					char sub[1024];
					int i = 0;
					int index;

					if (m_incoming[3] == '.' && m_incoming[4] == '.') {
						getcwd(sub, 50);
						while (sub[i] != '\0') {
							if (sub[i] == '/')
								index = i;
							i++;
						}
						sub[index] = '\0';
						chdir(sub);
						strcpy(m_outgoing, sub);
						result = sub;
					}
					else if (m_incoming[3] == '~') {
						getcwd(sub, 50);
						while (sub[i] != '\0') {
							if (sub[i] == '/') {
								index = i;
								if (sub[i + 1] == 'm' && sub[i + 2] == 'n' && sub[i + 3] == 't');
								else
									break;
							}
							i++;
						}
						sub[index] = '\0';
						chdir(sub);
						result = sub;
					}
					else {
						getcwd(sub, 50);
						while (m_incoming[i] != '\0') {
							i++;
						}
						int cnt = 0;
						while (sub[cnt] != '\0') {
							cnt++;
						}
						sub[cnt++] = '/';
						for (int k = 3; k < i; k++) {
							sub[cnt] = m_incoming[k];
							cnt++;
						}
						chdir(sub);
						result = sub;
					}
				}
				else if (m_incoming[0] == 'h' && m_incoming[1] == 'i' && m_incoming[2] == 's')
				{
					result = displayHistory();
				}
				else if (m_incoming[0] == '!')
				{
					if (m_incoming[1] - '0' - 1 < hist_num) {
						strcpy(m_incoming, history[(m_incoming[1] - '0' - 1)].c_str());
						//printf("%s\n", m_incoming); 현재 나오는 incoming 부분

						FILE* pipe2 = popen(m_incoming, "r");
						if (!pipe2) throw std::runtime_error("popen() failed!");
						try {
							while (!feof(pipe2)) {
								if (fgets(buffer, 1024, pipe2) != NULL) {
									result += buffer;
									//printf("%s", buffer);
								}
							}
						}
						catch (...) {
							pclose(pipe2);
							result = "Not Found";
							throw;
						}
						pclose(pipe2);

						addHistory(m_incoming);
					}
					else {
						printf("Not Found\n");
						result = "Not Found";
					}
				}
				else {
					addHistory(m_incoming);
				}
			}
			catch (...) {
				pclose(pipe);
				result = "Not Found";
				throw;
			}
			pclose(pipe);
			strcpy(m_outgoing, result.c_str());
			//cout << result;
		}

		send(m_sockfd, m_outgoing, strlen(m_outgoing), 0);
		if (strcmp(m_incoming, "exit") == 0)
		{
			printf("Shell terminated.\n");
			close(m_sockfd);
			break;
		}
	}
	return 0;
}

void Shell::addHistory(string newCmd)
{
	hist_num++; // increment number of commands in history
	history.push_back(newCmd); // insert new command to history vector
}

string Shell::displayHistory()
{
	string temp;
	temp.reserve(100);
	if (history.size() == 0) //If nothing in history than display error.
	{
		cout << "Error, no commands in history." << endl;
	}
	else //otherwise display the last ten commands used
	{
		int tenthCmd; // index for where history should stop displaying
		if (hist_num > 10) // if hist_num is greater than 10.
		{
			tenthCmd = hist_num - 10;// subtract 10 from hist_num.
		}
		else
		{
			tenthCmd = 0;
		}
		for (int nthCmd = tenthCmd; nthCmd <= hist_num - 1; nthCmd++) // will only display the last 10 items in history
		{
			cout << nthCmd + 1 << " " << history[nthCmd] << endl;
			temp.append(to_string(nthCmd + 1));
			temp.append(" ");
			temp.append(history[nthCmd]);
			temp.append("\n");
		}
	}// end of the else
	return temp;
}
