CXX = /usr/bin/g++ # compielr
CXXFLAGS = -lpthread

all : server client

server :
	$(CXX) -c server.cpp shell.cpp 
	$(CXX) -o server server.o shell.o $(CXXFLAGS)

client : 
	$(CXX) -c client.cpp 
	$(CXX) -o client client.o


.PHONY=clean	
clean:
	/bin/rm -f *.o server client shell
