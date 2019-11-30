#include "server.hpp"
#include "client.hpp"

using namespace RemoteShell;

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
    else if (strcmp(argv[1], "client")==0)
    {
        Client client;
    }
    else if (strcmp(argv[1], "client")==0)
    {
        Shell shell(atoi(argv[2]));
    }

    return 0;
}

