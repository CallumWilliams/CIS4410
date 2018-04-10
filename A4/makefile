all: client server

client: client-process.c comm-server.h client.h
	gcc client-process.c -o client

server: name-server.c name-server.h
	gcc name-server.c -o server

clean:
	rm server client
