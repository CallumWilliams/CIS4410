#include <stdio.h>

#include "comm-server.h"

void displayUI() {
	
	printf("What would you like to do?\n");
	printf("(1) Register a user\n");
	printf("(2) Deregister\n");
	printf("(3) Send message to a user\n");
	printf("(4) Block a user\n");
	printf("(5) Terminate the program\n");
	printf("> ");
	
}

void user1() {
	
	serv_register("Thomas");
	receive();
	serv_send("Bob", "Hello Bob");
	terminate("Thomas");
	
}

void user2() {
	
	serv_register("Bob");
	serv_send("Thomas", "Hi Tom");
	receive();
	terminate("Bob");
	
}
