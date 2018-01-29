#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

char FILE_STORAGE[10];//used to modify the file
int FILE_LENGTH;
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
	r.id = id;
	r.read_count = 0;
	r.wait_time = 1 + rand() % 1000;
	return r;
	
}

WRITERS setupWriter(int id) {
	
	WRITERS w;
	w.id = id;
	w.wait_time = 1 + rand() % 1000;
	return w;
	
}

void setupFile(int n) {
	
	int i;
	FILE_LENGTH = n;
	for (i = 0; i < n; i++) {
		FILE_STORAGE[i] = '0';
		printf("FILE: %s\n", FILE_STORAGE);
	}
	
	
}

void *readFromFile(void *p) {
	
	int readc = 0;
	READERS *r = (READERS *)p;
	int id = r->id;
	int wait = r->wait_time;
	int ERROR;
	
	while(readc < MAX_READ_COUNT) {
		
		usleep(wait);
		ERROR = pthread_mutex_trylock(&mutex1);
		if (ERROR == 0) {//lock is open
			printf("R%d has read:\t", id);
			for (int i = 0; i < FILE_LENGTH; i++) {
				printf("%c", FILE_STORAGE[i]);
			}
			printf("\n");
			readc++;
			pthread_mutex_unlock(&mutex1);
		}
		
	}
	printf("R%d done reading\n", id);
	
}

void *writeToFile(void *p) {
	
	WRITERS *w = (WRITERS *)p;
	int id = w->id;
	int wait = w->wait_time;
	int isDone = 0;
	int ERROR;
	char *tmp;
	
	do {
		
		usleep(wait);
		ERROR = pthread_mutex_trylock(&mutex1);
		if (ERROR == 0) {//lock is open
			FILE_STORAGE[id] = id + '0';
			printf("W%d wrote:\t", id);
			for (int i = 0; i < FILE_LENGTH; i++) {
				printf("%c", FILE_STORAGE[i]);
			}
			printf("\n");
			isDone = 1;
			pthread_mutex_unlock(&mutex1);
		}
		
	} while(!isDone);
	
}
