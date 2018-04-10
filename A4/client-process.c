#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "client.h"
#include "comm-server.h"

void main(int argc, char *argv[]) {
	
	int opt;
	char input[512];
	
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
					printf("Enter username > ");
					fgets(input, 512, stdin);
					serv_register(input);
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					printf("Terminating...\n");
					exit(1);
				
			}
			
		} else {
			printf("Invalid input\n");
		}
		
	} while(1);
	
	strcpy(buff, "Hello\n");
	
}

