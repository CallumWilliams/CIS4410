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

void main(int argc, char *argv[]) {
	
	int opt, toRun;
	char input[512];
	
	if (argc != 3) {
		printf("Please run as: ./client <port> <process-to-run>\n");
		exit(1);
	}
	
	initializeNetwork(atoi(argv[1]));
	toRun = atoi(argv[2]);
	
	switch (toRun) {
		
		case 1:
			user1();
			break;
		case 2:
			user2();
			break;
		default:
			printf("invalid value %d\n", toRun);
	}
	
}

