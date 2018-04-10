#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "name-server.h"

void main(int argc, char *argv[]) {
	
	int port;
	int sockfd;
	struct sockaddr_in si_me, si_other;
	char buff[512];
	socklen_t addr_size;
	
	initializeNamesList();
	
	if (argc != 2) {
		printf("Please run as: ./server <port>\n");
		exit(1);
	}
	
	port = atoi(argv[1]);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&si_me, '\0', sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(port);
	si_me.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	bind(sockfd, (struct sockaddr *)&si_me, sizeof(si_me));
	addr_size = sizeof(si_other);
	
	//main loop
	do {
		
		char *tok;
		
		recvfrom(sockfd, buff, 512, 0, (struct sockaddr *) &si_other, &addr_size);
		buff[strlen(buff)-1] = '\0';
		
		tok = strtok(buff, " ");
		if (strcmp(tok, "REGISTER") == 0) {
			
			tok = strtok(NULL, " ");
			printf("REGISTER %s from %d\n", tok, si_other.sin_port);
			
		} else {
			
			printf("unexpected token %s\n", tok);
			
		}
		
	} while(1);
	
}
