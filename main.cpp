#include "server.hpp"
#include "client.hpp"

#include <cstdio>
#include <cstring>

using namespace std;

int main(int argc, char** argv){
    if (argc < 2)
    {
      printf("Usage : RemoteShell [server|client]\n");
      return -1;
    }

    if (strcmp(argv[1], "server")==0)
    {
        Server server;
    }

  return 0;
}

