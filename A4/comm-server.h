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
char buff_in[512];
socklen_t addr_size;

char client_username[50];
int REGISTERED_FLAG = 0;

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
	printf("Successfully registered %s\n", buff);
	
}

void terminate(char *process_name) {
	
	if (REGISTERED_FLAG) {
		REGISTERED_FLAG = 0;
		sprintf(buff, "TERM %s", client_username);
		sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &servAddr, sizeof(servAddr));
		printf("Successfully deregistered %s\n\n", client_username);
		memset(client_username, '\0', 50);
	} else {
		printf("No client registered.\n\n");
	}
	
}

void serv_send(char *process_name, char *msg) {
	
	sprintf(buff, "SEND %s %s", process_name, msg);
	sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *) &servAddr, sizeof(servAddr));
	
}

int receive(char *msg) {
	
	return recvfrom(sockfd, msg, 512, 0, (struct sockaddr *) &servAddr, &addr_size);
	
}

int block_receive(char *process_name, char *msg) {
	
	
	
}

void *serverListen(void *p) {
	
	memset(buff_in, '\0', 512);
	while (receive(buff_in) != -1) {
		printf("%s\n", buff_in);
		memset(buff_in, '\0', 512);
	}
	
}
