DEPENDENCIES=-pthread

all: client server

client: client.c client.h
	gcc $(DEPENDENCIES) client.c -o client

server: server.c server.h
	gcc $(DEPENDENCIES) server.c -o server
	
clean:
	rm client
	rm server
	clear
