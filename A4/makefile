all: client server

client: client-process.c
	gcc client-process.c -o client

server: name-server.c
	gcc name-server.c -o server

clean:
	rm server client
