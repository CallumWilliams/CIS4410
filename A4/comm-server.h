#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int port;
int sockfd;
struct sockaddr_in servAddr;
char buff[512];
socklen_t addr_size;

void initializeNetwork(int p) {
	
	port = p;
	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	
	memset(&servAddr, '\0', sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
}

void serv_register(char *process_name) {
	
	REGISTERED_FLAG = 1;
	strcpy(client_username, process_name);
	sprintf(buff, "REGISTER %s", process_name);
	sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &servAddr, sizeof(servAddr));
	printf("Sent %s\n", buff);
	
}

void terminate(char *process_name) {
	
	if (REGISTERED_FLAG) {
		REGISTERED_FLAG = 0;
		memset(client_username, '\0', 50);
	} else {
		printf("No client registered.\n\n");
	}
	
}

void serv_send(char *process_name, char *msg) {
	
	
	
}

int receive(char *process_name, char *msg) {
	
	return 0;
	
}

int block_receive(char *process_name, char *msg) {
	
	
	
}
