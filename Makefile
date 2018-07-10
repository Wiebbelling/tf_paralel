all:
	g++ -std=c++0x -lpthread -g atmServer.cpp -o server
	g++ client.cpp -o client
client:
	g++ client.cpp -o client
server:
	g++ -std=c++0x -lpthread -g atmServer.cpp -o server

clean:
	rm server
	rm client