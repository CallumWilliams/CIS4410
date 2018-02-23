#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.h"

int main(int argc, char *argv[]) {
	
	/*2D array of readers/writers - 1 dimension for group number and 1 
	dimension for amount in group*/
	READERS **r;
	WRITERS **w;
	pthread_t **rp, **wp;
	
	int groupCount;
	int *readersPerGroup;
	int *writersPerGroup;
	
	if (argc < 2) {
		printf("Run as \"./client (<number-of-readers>-<number-of-writers>) x(number-of-groups)\"\n");
		exit(1);
	}
	
	groupCount = argc - 1;
	readersPerGroup = malloc(sizeof(int)*groupCount);
	writersPerGroup = malloc(sizeof(int)*groupCount);
	r = malloc(sizeof(READERS *)*groupCount);
	w = malloc(sizeof(WRITERS *)*groupCount);
	rp = malloc(sizeof(pthread_t *)*groupCount);
	wp = malloc(sizeof(pthread_t *)*groupCount);
	FILE_STORAGE = malloc(sizeof(char *)*groupCount);
	ACCEPT_MESSAGES = malloc(sizeof(char *)*groupCount);
	
	if (!readersPerGroup || !writersPerGroup || !r || !w || !rp || !wp || !FILE_STORAGE) {
		printf("Insufficient memory\n");
		exit(1);
	}
	
	/*loop through the command-line arguments and generate our
	groups of readers/writers*/
	for (int i = 0; i < groupCount; i++) {
		
		readersPerGroup[i] = atoi(strtok(argv[i+1], "-"));
		writersPerGroup[i] = atoi(strtok(NULL, "-"));
		setupFile(i, writersPerGroup[i]);
		ACCEPT_MESSAGES[i] = malloc(sizeof(char)*20);
		r[i] = malloc(sizeof(READERS)*readersPerGroup[i]);
		w[i] = malloc(sizeof(WRITERS)*writersPerGroup[i]);
		rp[i] = malloc(sizeof(pthread_t)*readersPerGroup[i]);
		wp[i] = malloc(sizeof(pthread_t)*writersPerGroup[i]);
		if (!r[i] || !w[i] || !rp[i] || !wp[i]) {
			printf("Insufficient memory\n");
			exit(1);
		}
		
	}
	
	//setup all readers and writers
	srand(time(NULL));
	for (int i = 0; i < groupCount; i++) {
		
		//readers
		for (int j = 0; j < readersPerGroup[i]; j++) {
			r[i][j] = setupReader(i, j);
			pthread_create(&rp[i][j], NULL, startReader, (void *) &r[i][j]);
		}
		
		//writers
		for (int j = 0; j < writersPerGroup[i]; j++) {
			w[i][j] = setupWriter(i, j);
			pthread_create(&wp[i][j], NULL, startWriter, (void *) &w[i][j]);
		}
		
	}
	
	//active threads
	for (int i = 0; i < groupCount; i++) {
		
		for (int j = 0; j < readersPerGroup[i]; j++) {
			pthread_join(rp[i][j], NULL);
		}
		
		for (int j = 0; j < writersPerGroup[i]; j++) {
			pthread_join(wp[i][j], NULL);
		}
		
	}
	
	//free memory
	for (int i = 0; i < groupCount; i++) {
		
		free(r[i]);
		free(w[i]);
		free(rp[i]);
		free(wp[i]);
		
	}
	free(r);
	free(w);
	free(rp);
	free(wp);
	free(readersPerGroup);
	free(writersPerGroup);
	printf("All clients done - terminating\n");
	
	return 0;

}
