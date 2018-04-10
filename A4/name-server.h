#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _name {
	
	char *usr_name;
	int usr_socket;
	
} NAME;

NAME USERS[100];
int nextIndex = 0;

void initializeNamesList() {
	
	for (int i = 0; i < 100; i++) {
		USERS[i].usr_name = malloc(sizeof(char)*50);
		memset(USERS[i].usr_name, '\0', 50);	
	}
	
}

void addUserToNamesList(char *usr_n, int usr_s) {
	
	NAME newUser;
	
	strcpy(newUser.usr_name, usr_n);
	newUser.usr_socket = usr_s;
	USERS[nextIndex] = newUser;
	
	nextIndex++;
	
}

int searchForUserWithName(char *toFind) {
	
	int i;
	
	for (i = 0; i < nextIndex; i++) {
		
		if (strcmp(toFind, USERS[i].usr_name) == 0) {
			return i;
		}
		
	}
	
	return -1;
	
}

void removeUserFromNamesList(char *toRem) {
	
	int i, ind;
	
	ind = searchForUserWithName(toRem);
	
	if (ind != -1) {
		
		//shift all users after the found index back one
		for (i = ind; i < nextIndex+1; i++) {
			strcpy(USERS[i].usr_name, USERS[i+1].usr_name);
			USERS[i].usr_socket = USERS[i+1].usr_socket;
		}
		nextIndex--;
		
	} else {
		
		printf("User %s not found\n", toRem);
		
	}
	
}
