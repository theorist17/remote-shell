#include "shell.hpp"
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>

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

int Shell::run()
{
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
			std::string output = "";
			FILE *pipe = popen(m_incoming, "r");
			if (!pipe)
				throw std::runtime_error("popen() failed!");
			try
			{
				output += " ";
				while (!feof(pipe))
				{
					if (fgets(buffer, 1024, pipe) != NULL)
					{
						output += buffer;
					}
				}
				if (m_incoming[0] == 'c' && m_incoming[1] == 'd')
				{
					char sub[1024] = " ";
					int i = 0;
					int index;

					if (m_incoming[3] == '.' && m_incoming[4] == '.')
					{
						getcwd(sub, 50);
						while (sub[i] != '\0')
						{
							if (sub[i] == '/')
								index = i;
							i++;
						}
						sub[index] = '\0';
						chdir(sub);
						strcpy(m_outgoing, sub);
						output = sub;
					}
					else if (m_incoming[3] == '~')
					{
						getcwd(sub, 50);
						while (sub[i] != '\0')
						{
							if (sub[i] == '/')
							{
								index = i;
								if (sub[i + 1] == 'm' && sub[i + 2] == 'n' && sub[i + 3] == 't')
									;
								else
									break;
							}
							i++;
						}
						sub[index] = '\0';
						chdir(sub);
						output = sub;
					}
					else
					{
						getcwd(sub, 50);
						while (m_incoming[i] != '\0')
						{
							i++;
						}
						int cnt = 0;
						while (sub[cnt] != '\0')
						{
							cnt++;
						}
						sub[cnt++] = '/';
						for (int k = 3; k < i; k++)
						{
							sub[cnt] = m_incoming[k];
							cnt++;
						}
						chdir(sub);
						output = sub;
					}
				}
				else if (m_incoming[0] == 'h' && m_incoming[1] == 'i' && m_incoming[2] == 's')
				{
					output = displayHistory();
				}
				else if (m_incoming[0] == '!')
				{
					if (m_incoming[1] - '0' - 1 < hist_num)
					{
						strcpy(m_incoming, history[(m_incoming[1] - '0' - 1)].c_str());

						FILE *pipe2 = popen(m_incoming, "r");
						if (!pipe2)
							throw std::runtime_error("popen() failed!");
						try
						{
							while (!feof(pipe2))
							{
								if (fgets(buffer, 1024, pipe2) != NULL)
								{
									output += buffer;
								}
							}
						}
						catch (...)
						{
							pclose(pipe2);
							output = "Not Found";
							throw;
						}
						pclose(pipe2);

						addHistory(m_incoming);
					}
					else
					{
						output = "Not Found";
					}
				}
                else if  (!string(m_incoming).substr(0, 8).compare("download"))
                {
                    string filename = string(m_incoming).substr(9);
                    int fd, bytesread, size = 1024;
                    char buf[1024];
                    while( fd = open(filename.c_str(), O_RDONLY), fd == -1 && errno == EINTR);
                    while( bytesread = read(fd, buf, size), bytesread == -1 && errno == EINTR);
                    output = string(buf);
                    close(fd);
                    addHistory(m_incoming);
                }
                else if  (!string(m_incoming).substr(0, 6).compare("upload"))
                {
                    int fd;
                    string target = string(m_incoming).substr(7);
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
                                target = target.substr(0, ind).append("_").append(ext);
                            }
                        }
                    }

                    addHistory(m_incoming);

                    memset(m_incoming, 0, 1024);
		            int bytesrecv = recv(m_sockfd, m_incoming, 1024, 0);

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
                        {
                            char msg_buf[1024] = "upload failed";
		                    send(m_sockfd, msg_buf, strlen(msg_buf), 0);
                        }
                        if (byteswritten == -1)
                            byteswritten = 0;
                        totalbytes += byteswritten;
                    }
                    close(fd);

		            send(m_sockfd, target.c_str(), target.length(), 0);
                    continue;
                }
				else
				{
					addHistory(m_incoming);
				}
			}
			catch (...)
			{
				pclose(pipe);
				output = "Not Found";
				throw;
			}
			pclose(pipe);
			strcpy(m_outgoing, output.c_str());
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
	hist_num++;				   // increment number of commands in history
	history.push_back(newCmd); // insert new command to history vector
}

string Shell::displayHistory()
{
	string temp;
	temp.reserve(100);
	if (history.size() == 0) //If nothing in history than display error.
	{
		cout << "Error, no commands in history." << endl;
		temp = " ";
		return temp;
	}
	else //otherwise display the last ten commands used
	{
		int tenthCmd;	  // index for where history should stop displaying
		if (hist_num > 10) // if hist_num is greater than 10.
		{
			tenthCmd = hist_num - 10; // subtract 10 from hist_num.
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
	} // end of the else
	return temp;
}
