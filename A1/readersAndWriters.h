#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

char *FILE_STORAGE;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int MAX_READ_COUNT = 10;

typedef struct rea {
	
	int id;
	int read_count;
	float wait_time;
	
}READERS;

typedef struct wri {
	
	int id;
	float wait_time;
	
}WRITERS;

READERS setupReader(int id) {
	
	READERS r;
	srand(time(NULL));
	r.id = id;
	r.read_count = 0;
	r.wait_time = rand();
	return r;
	
}

WRITERS *setupWriter(int id) {
	
	WRITERS *w = malloc(sizeof(WRITERS));
	srand(time(NULL));
	w->id = id;
	w->wait_time = rand();
	return w;
	
}

char *setupFile(int n) {
	
	int i;
	char *f = malloc(sizeof(char)*n+1);
	for (i = 0; i < n; i++) {
		f = strcat(f, "0");
	}
	
	return f;
	
}

void *readFromFile(void *p) {
	
	int readc = 0;
	READERS *r = (READERS *)p;
	int id = r->id;
	int wait = r->wait_time;
	
	while(readc < MAX_READ_COUNT) {
		
		//pthread_mutex_trylock(&mutex1);
		printf("R%d has read:\t%s\n", id, FILE_STORAGE);
		readc++;
		//pthread_mutex_unlock(&mutex1);
		
	}
	
}
/*
void *writeToFile(void *p) {
	
	//get lock
	//write to file
	//release lock
	//return string
	
}
*/
