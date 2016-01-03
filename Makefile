all: client server

client: client.cpp shq.cpp sem.cpp sem.h shq.h
	g++ --std=c++11 -O2 client.cpp shq.cpp sem.cpp -o client

server: server.cpp shq.cpp sem.cpp sem.h shq.h
	g++ --std=c++11 -O2 server.cpp shq.cpp sem.cpp -o server