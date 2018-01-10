#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "readersAndWriters.h"

int main(int argc, char **argv) {
	
	int n, m;
	int i;
	READER *readers;
	WRITER *writers;
	
	if (argc < 3) {
		printf("Insufficient arguments - please run as './readersAndWriters <N value> <M value>\n");
		return 1;
	}
	
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	
	readers = malloc(sizeof(READER)*n);
	writers = malloc(sizeof(WRITER)*m);
	
	if (!readers || !writers) {
		printf("ERROR - Insufficient memory for readers/writers.\n");
		return 1;
	}
	
	srand(time(NULL));
	
	for (i = 0; i < n; i++) {
		
		readers[i].READER_ID = i;
		readers[i].WAIT_DURATION = rand() % 100 + 1;
		printf("Reader %d waits for %d seconds\n", readers[i].READER_ID, readers[i].WAIT_DURATION);
		
	}
	
	for (i = 0; i < m; i++) {
		
		writers[i].WRITER_ID = i;
		writers[i].WAIT_DURATION = rand() % 100 + 1;
		printf("Writer %d waits for %d seconds\n", writers[i].WRITER_ID, writers[i].WAIT_DURATION);

		
	}
	
	return 0;
	
}
