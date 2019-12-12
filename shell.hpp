#ifndef _SHELL
#define _SHELL
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h> 
#include <sys/socket.h>
#include <vector>
#include <limits.h> // PATH_MAX

namespace RemoteShell {
	using namespace std;
	class Shell {
	private:
		int m_sockfd;
		char m_incoming[1024];
		char m_outgoing[1024];
		char buffer[1024];

		int hist_num = 0;
		bool runBackground = false;
		vector<string> history; // vector of commands entered.
	public:
		Shell();
		Shell(int sockfd);
		~Shell();
		int run();
		void addHistory(string newCmd);
		string displayHistory();
	};
}
#endif
