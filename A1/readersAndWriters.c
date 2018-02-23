#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "readersAndWriters.h"

int main(int argc, char *argv[]) {
	
	READERS *readers;
	WRITERS *writers;
	int m, n;
	int i;
	int *arg;
	pthread_t *rp, *wp;
	READERS *rtmp;
	WRITERS *wtmp;
	int ret1, ret2;
	char *msg1 = "test1";
	char *msg2 = "test2";
	
	if (argc < 3) {
		printf("Please run as './readersAndWriters <# of readers> <# of writers>\n");
		exit(1);
	}
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	
	readers = malloc(sizeof(READERS)*n);
	writers = malloc(sizeof(WRITERS)*m);
	rtmp = malloc(sizeof(READERS));
	wtmp = malloc(sizeof(WRITERS));
	
	if (!readers || !writers || !rtmp || !wtmp) {
		printf("Insufficient memory\n");
		exit(1);
	}
	setupFile(m);
	rp = malloc(sizeof(pthread_t)*n);
	wp = malloc(sizeof(pthread_t)*m);
	arg = malloc(sizeof(int));
	if (!rp || !wp || !arg) {
		printf("Insufficient memory for threads\n");
		exit(1);
	}
	
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		
		readers[i] = setupReader(i);
		
	}
	
	for (i = 0; i < m; i++) {
		
		writers[i] = setupWriter(i);
		
	}
	
	for (i = 0; i < n; i++) {
		
		rtmp = &readers[i];
		pthread_create(&rp[i], NULL, readFromFile, (void *)rtmp);
		
	}
	
	for (i = 0; i < m; i++) {
		
		wtmp = &writers[i];
		pthread_create(&wp[i], NULL, writeToFile, (void *)wtmp);
		
	}
	
	for (i = 0; i < n; i++) {
		
		pthread_join(rp[i], NULL);
		
	}
	
	for (i = 0; i < m; i++) {
		
		pthread_join(wp[i], NULL);
		
	}
	
	return 0;
	
}
