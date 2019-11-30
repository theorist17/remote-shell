CXX = /usr/bin/g++ # compielr
PROG = RemoteShell
SRC = client.cpp server.cpp

all : server client
	$(CXX) -c main.cpp
	$(CXX) -o $(PROG) main.o server.o client.o

server : 
	$(CXX) -c server.cpp

client : 
	$(CXX) -c client.cpp

.PHONY=clean	
clean:
	/bin/rm -f *.o $(PROG)
