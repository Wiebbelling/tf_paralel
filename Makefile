all:
	g++ -g atmServer.cpp -o server
	g++ client.cpp -o client
client:
	g++ client.cpp -o client
server:
	g++ -g atmServer.cpp -o server

clean:
	rm server
	rm client