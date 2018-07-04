all:
	g++ atm.cpp -o server
	g++ client.cpp -o client
client:
	g++ client.cpp -o client
server:
	g++ atm.cpp -o server

clean:
	rm server
	rm client