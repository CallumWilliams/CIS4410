#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define PORT 9000
#define MAX_READ_COUNT 3

char **ACCEPT_MESSAGES;
char **FILE_STORAGE;
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

/**setup FILE_STORAGE[i] to be g characters long (and makes it all blank**/
void setupFile(int i, int g) {
	
	FILE_STORAGE[i] = malloc(sizeof(char)*g);
	for (int j = 0; j < g; j++) {
		FILE_STORAGE[i][j] = '0';
	}
	
}

/**structure for readers**/
typedef struct _reader {
	
	int group;
	int id;
	int read_count;
	float wait_time;
	
}READERS;

/**structure for writers**/
typedef struct _writer {
	
	int group;
	int id;
	float wait_time;
	
}WRITERS;

/**establishes a reader**/
READERS setupReader(int g, int id) {
	
	READERS r;
	r.group = g;
	r.id = id;
	r.read_count = 0;
	r.wait_time = (10 + rand() % 990) * 100;
	return r;
	
}

/**establishes a writer**/
WRITERS setupWriter(int g, int id) {
	
	WRITERS w;
	w.group = g;
	w.id = id;
	w.wait_time = (10 + rand() % 990) * 100;
	return w;
	
}

/**start thread for reader**/
void *startReader(void *p) {
	
	int clientSocket, portNum, nBytes;
	char buff_in[20];
	char buff_out[20];
	struct 	 sockaddr_in serverAddr;
	socklen_t addr_size;
	
	READERS *r = (READERS *)p;
	int group = r->group;
	int id = r->id;
	float wait = r->wait_time;
	int readc = 0;
	
	memset(buff_in, '\0', 20);
	memset(buff_out, '\0', 20);
	
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	
	portNum = 7891;
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
	
	addr_size = sizeof(serverAddr);
	
	do {
		
		usleep(wait);
	
		connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
		
		sprintf(buff_out, "REQUEST %d R %d", group, id);
		buff_out[strlen(buff_out)] = '\0';
		
		send(clientSocket, buff_out, strlen(buff_out) + 1, 0);
		
		//wait until client receives something from server
		recv(clientSocket, buff_in, sizeof(buff_in), 0);
		
		//check if error was returned from recv
		if (strcmp(strtok(buff_in, " "), "ERROR") == 0) {
			printf("GROUP %d R%d received an error. Run server with more groups.\n", group, id);
			exit(0);
		}
		
		close(clientSocket);
		
		//do reading
		printf("(Group %d) R%d read:\t%s (%d)\n", group, id, FILE_STORAGE[group], readc);
		
		readc++;
		//breakout condition
		if (readc == MAX_READ_COUNT) {
			//send release with terminate flag
			sprintf(buff_out, "RELEASE %d R %d -t", group, id);
			buff_out[strlen(buff_out)] = '\0';
			
			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
			if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) < 0) {
				printf("Reconnection error\n");
			}
			
			printf("Sending back %s\n", buff_out);
			
			send(clientSocket, buff_out, strlen(buff_out) + 1, 0);
		} else {
			//send release without terminate flag
			sprintf(buff_out, "RELEASE %d R %d", group, id);
			buff_out[strlen(buff_out)] = '\0';
			
			clientSocket = socket(PF_INET, SOCK_STREAM, 0);
			if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) < 0) {
				printf("Reconnection error\n");
			}
			
			printf("Sending back %s\n", buff_out);
			
			send(clientSocket, buff_out, strlen(buff_out) + 1, 0);
		}
		close(clientSocket);
		
	} while (readc < MAX_READ_COUNT);
	
	//build new socket and request release + termination
	
	
}

/**start thread for writer**/
void *startWriter(void *p) {
	
	int clientSocket, portNum, nBytes;
	char buff_in[20];
	char buff_out[20];
	struct 	 sockaddr_in serverAddr;
	socklen_t addr_size;
	
	WRITERS *w = (WRITERS *)p;
	int group = w->group;
	int id = w->id;
	float wait = w->wait_time;
	
	memset(buff_in, '\0', 20);
	memset(buff_out, '\0', 20);
	
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	
	portNum = 7891;
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));
	
	addr_size = sizeof(serverAddr);
	
	usleep(wait);
	
	connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
	
	sprintf(buff_out, "REQUEST %d W %d", group, id);
	buff_out[strlen(buff_out)] = '\0';
	
	printf("%d %d sent %s\n", group, id, buff_out);
	send(clientSocket, buff_out, strlen(buff_out) + 1, 0);
	
	//wait until client receives something from server
	recv(clientSocket, buff_in, sizeof(buff_in), 0);
	
	//check if error was returned from recv
	if (strcmp(strtok(buff_in, " "), "ERROR") == 0) {
		printf("(GROUP %d) R%d received an error. Run server with more groups.\n", group, id);
		exit(0);
	}
	
	close(clientSocket);
	
	printf("%d %d got back %s\n", group, id, buff_in);
	
	//do writing
	FILE_STORAGE[group][id] = id + '0';
	printf("(Group %d) W%d wrote:\t%s\n", group, id, FILE_STORAGE[group]);
	
	//build new socket and request release + termination
	sprintf(buff_out, "RELEASE %d W %d -t", group, id);
	buff_out[strlen(buff_out)] = '\0';
	
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size) < 0) {
		printf("Reconnection error\n");
	}
	
	printf("Sending back %s\n", buff_out);
	
	send(clientSocket, buff_out, strlen(buff_out) + 1, 0);
	
	close(clientSocket);
	
}
