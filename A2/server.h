#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7891

/**structure for all currently open connections**/
typedef struct _conn {
	
	int socketID;//associated socket with connection
	//associated infortmation with connection (for searching)
	int group;
	char type;
	int id;
	
}CONNECTION;

/**global list of all currently active connections**/
CONNECTION activeConnections[100];
/**global variable for number of active connections**/
int connectionCount = 0;

/**structure for a incoming request**/
typedef struct _request {
	
	int group;
	int id;
	char type;
		
}REQUEST;

/**generates a request for entry in the queue**/
REQUEST generateRequest(int g, char t, int id) {
	
	REQUEST r;
	r.group = g;
	r.id = id;
	r.type = t;
	return r;
	
}

/**initializes a connection element**/
CONNECTION initializeConnection(int sockid, int gr, char t, int id) {
	
	CONNECTION c;
	c.socketID = sockid;
	c.group = gr;
	c.type = t;
	c.id = id;
	return c;
	
}

/**Add connection toAdd to the active list**/
void addConnection(CONNECTION toAdd) {
	
	activeConnections[connectionCount] = toAdd;
	connectionCount++;
	
}

/**Compares the group, type and id of a connection and request**/
int compareConnectionAndRequest(CONNECTION c, REQUEST r) {
	
	if ((c.group == r.group) && (c.type == r.type) && (c.id == r.id)) return 1;
	return 0;
	
}

/**removes connection c from the list of active ones**/
void removeConnection(REQUEST toFind) {
	
	int i;
	
	//find
	for (i = 0; i < connectionCount; i++) {
		if (compareConnectionAndRequest(activeConnections[i], toFind) == 1) {
			
			break;
			
		}
	}
	
	//remove
	for (i = i + 1; i < connectionCount; i++) {
		activeConnections[i-1] = activeConnections[i];
	}
	connectionCount--;
	
}

/**Find and return the connection that matches toFind**/
CONNECTION searchForConnection(REQUEST toFind) {
	
	CONNECTION ret;
	int i;
	
	for (i = 0; i < connectionCount; i++) {
		if (compareConnectionAndRequest(activeConnections[i], toFind) == 1) {
			
			ret = activeConnections[i];
			break;
			
		}
	}
	
	return ret;
	
}

/**simple queue structure**/
typedef struct _queue {
	
	REQUEST element[100];
	int element_count;
	
}QUEUE;

/**initializes a queue**/
QUEUE initializeQueue() {
	
	QUEUE q;
	q.element_count = 0;
	return q;
	
}

/**add an element to the back of a queue**/
QUEUE addToQueue(QUEUE q, REQUEST e) {
	
	q.element[q.element_count] = e;
	q.element_count++;
	return q;
	
}

/**gets first element of q**/
REQUEST getFirstElement(QUEUE q) {
	
	return q.element[0];
	
}

/**remove from element of queue and move rest of queue forward**/
QUEUE removeFrontElement(QUEUE q) {
	
	for (int i = 1; i < q.element_count; i++) {
		q.element[i-1] = q.element[i];
	}
	q.element_count--;
	return q;
	
}

/**structure for server agent (used to handle all in-group requests)**/
typedef struct _agent {
	
	int group;
	QUEUE q;
	int busy;//0 = not busy, 1 = busy
	
}SERV_AGENT;

/**initial setup for a server agent for group g**/
SERV_AGENT initializeAgent(int g) {
	
	SERV_AGENT s;
	s.group = g;
	s.q = initializeQueue();
	s.busy = 0;
	return s;
	
}

/**main control loop for the server**/
void startServer(int g) {
	
	SERV_AGENT *s;
	
	struct linger l;
	int welcomeSocket, newSocket;
	char buff_in[20];
	char buff_out[20];
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	int i;
	int readerCount = 0;
	
	pid_t child;
	int status;
	
	//pre-server setup
	memset(buff_in, '\0', 20);
	memset(buff_out, '\0', 20);
	
	s = malloc(sizeof(SERV_AGENT)*g);
	if (!s) {
		printf("Insufficient memory\n");
		exit(1);
	}
	
	for (i = 0; i < g; i++) {
		s[i] = initializeAgent(i);
	}
	
	//disable linger
	l.l_onoff = 0;
	l.l_linger = 0;
	
	//establish connection
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
	
	//apply linger changes
	if (setsockopt(welcomeSocket, SOL_SOCKET, SO_LINGER, (char *) &l, sizeof(l)) < 0) {
		printf("Sockopt error\n");
		exit(1);
	}
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
	
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	
	if(listen(welcomeSocket,200)==0)
		printf("Listening\n");
	else
		printf("Error\n");
	
	addr_size = sizeof(serverStorage);
	
	//main control loop
	while(1){
		
		newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
		
		if (recv(newSocket, buff_in, sizeof(buff_in), 0) < 0) {
			printf("Error receiving input. Terminated.\n");
			exit(1);
		}
		
		child = fork();
		if (!child) {
			
			printf("Received %s with connection %d\n", buff_in, newSocket);
			
			char *tok = strtok(buff_in, " ");
			if (strcmp(tok, "REQUEST") == 0) {
				
				int gr, id;
				char t;
				gr = atoi(strtok(NULL, " "));
				t = strtok(NULL, " ")[0];
				id = atoi(strtok(NULL, " "));
				//check gr to be sure it's in range 0..g
				if (gr < 0 || gr >= g) {//send back error
					printf("Group number %d is out of range\n", gr);
					sprintf(buff_out, "ERROR");
					buff_out[strlen(buff_out)] = '\0';
					send(newSocket, buff_out, strlen(buff_out) + 1, 0);
				} else {
					exit(1);//exit code for approved request
				}
				
			} else if (strcmp(tok, "RELEASE") == 0) {
				int gr, id;
				char t;
				gr = atoi(strtok(NULL, " "));
				t = strtok(NULL, " ")[0];
				id = atoi(strtok(NULL, " "));
				//check gr to be sure it's in range 0..g
				if (gr < 0 || gr >= g) {//send back error
					sprintf(buff_out, "ERROR");
					buff_out[strlen(buff_out)] = '\0';
					send(newSocket, buff_out, strlen(buff_out) + 1, 0);
					exit(0);
				} else {
					exit(2);
				}
			}
			
			exit(0);//bad error code
			
		} else {
			
			waitpid(child, &status, 0);
			
			if (status == 0) {//error was found. Terminate server.
				printf("Terminating due to error.\n");
				exit(0);
			} else if (status == 256) {//re-parse REQUEST info to build object
				
				int gr, id;
				char t;
				char *tok = strtok(buff_in, " ");
				gr = atoi(strtok(NULL, " "));
				t = strtok(NULL, " ")[0];
				id = atoi(strtok(NULL, " "));
				REQUEST new = generateRequest(gr, t, id);
				s[gr].q = addToQueue(s[gr].q, new);
				//add connection to the list of active ones
				CONNECTION c = initializeConnection(newSocket, gr, t, id);
				addConnection(c);
				
			} else if (status == 512) {//re-parse RELEASE info
				
				int gr, id;
				char t;
				char *tok = strtok(buff_in, " ");
				gr = atoi(strtok(NULL, " "));
				t = strtok(NULL, " ")[0];
				id = atoi(strtok(NULL, " "));
				if (strtok(NULL, " ") == NULL) {
					s[gr].busy = 0;//unlock s[gr]
				} else {//also terminate if flag is there
					s[gr].busy = 0;
					//find the connection related to tmp and close it
					REQUEST tmp = generateRequest(gr, t, id);
					CONNECTION c = searchForConnection(tmp);
					removeConnection(tmp);
					printf("%s closing %d\n", buff_in, c.socketID);
					if (t == 'R') readerCount--;
					if (readerCount == 0) s[gr].busy = 0;//open up busy queue
					close(c.socketID);
					close(newSocket);//also close current connection
				}
				
			}
			
			/*go through each server agent and find out if they are
			 *busy and have something to do in the queue*/
			for (int i = 0; i < g; i++) {
				
				if (s[i].busy == 2 && s[i].q.element[0].type == 'R') {
					
					readerCount++;
					printf("New readers %d\n", readerCount);
					REQUEST pop = getFirstElement(s[i].q);
					s[i].q = removeFrontElement(s[i].q);
					sprintf(buff_out, "ACCEPT %d %c %d", pop.group, pop.type, pop.id);
					buff_out[strlen(buff_out)] = '\0';
					//find the socket in the list of stored ones
					CONNECTION c = searchForConnection(pop);
					send(c.socketID, buff_out, strlen(buff_out) + 1, 0);
					printf("Replying to connection %d with %s\n", c.socketID, buff_out);
					
				//if queue isn't busy, an element is in the queue, and there are no active readers
				} else if (s[i].busy == 0 && s[i].q.element_count > 0 && readerCount == 0) {
					
					s[i].busy = 1;
					REQUEST pop = getFirstElement(s[i].q);
					s[i].q = removeFrontElement(s[i].q);
					//
					if (pop.type == 'R') {
						readerCount++;
						s[i].busy = 2;//set with reader flag
					}
					sprintf(buff_out, "ACCEPT %d %c %d", pop.group, pop.type, pop.id);
					buff_out[strlen(buff_out)] = '\0';
					//find the socket in the list of stored ones
					CONNECTION c = searchForConnection(pop);
					send(c.socketID, buff_out, strlen(buff_out) + 1, 0);
					printf("Replying to connection %d with %s\n", c.socketID, buff_out);
					
				}
				
			}
			
		}
		
	}
	
}
