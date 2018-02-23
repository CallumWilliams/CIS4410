#include <stdio.h>
#include <stdlib.h>

#include "server.h"

int main(int argc, char *argv[]){
	
	int g;
	
	if (argc < 2) {
		printf("Run as \"./server <number-of-groups>\"\n");
		exit(1);
	}
	
	g = atoi(argv[1]);
	
	startServer(g);
	
	return 0;
	
}
