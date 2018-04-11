#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <pthread.h>

#include "client.h"
#include "comm-server.h"

void main(int argc, char *argv[]) {
	
	int opt;
	char input[512];
	pthread_t listen;
	
	if (argc != 2) {
		printf("Please run as: ./server <port>\n");
		exit(1);
	}
	
	initializeNetwork(atoi(argv[1]));
	
	do {
		
		displayUI();
		fgets(input, 512, stdin);
		
		if (isdigit(input[0])) {
			
			opt = atoi(&input[0]);
			switch(opt) {
				
				case 1:
					if (REGISTERED_FLAG) {
						printf("Account already registered - please terminate to create new profile\n\n");
					} else {
						printf("Enter username > ");
						fgets(input, 512, stdin);
						serv_register(input);
						//start server listen
						pthread_create(&listen, NULL, serverListen, NULL);
					}
					break;
				case 2:
					terminate(client_username);
					break;
				case 3:
					printf("Who are you sending to > ");
					char usr[50]; 
					fgets(usr, 50, stdin);
					printf("What is your message > ");
					fgets(input, 512, stdin);
					serv_send(usr, input);
					break;
				case 4:
					break;
				case 5:
					if (REGISTERED_FLAG) {
						terminate(client_username);
					}
					printf("Terminating...\n");
					exit(1);
				
			}
			
		} else {
			printf("Invalid input\n");
		}
		
	} while(1);
	
	strcpy(buff, "Hello\n");
	
}

