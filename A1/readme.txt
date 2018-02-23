Name: Callum Williams
Date: January 29th, 2018
For: CIS4410

Compile:
	make

Run:
	./readersAndWriters <# of readers> <# of writers>

Clean:
	make clean

Alogirthm:
1. Generate all of the reader/writer objects and their threads.
2. Start them all at once (each with a random wait time)
For each thread:
	Check the mutex lock. If it is available, perform your operation and then 	  unlock it. Otherwise, wait for your wait_time and try again later.
Keep going until all the readers have reached their read limit and all the writers have changed the file.

Assumptions/Limitations:
 - since each record is a single character, it is assumed that the maximum amount of writers is 10 (0 - 9), as otherwise the system would use different characters for further records.
 - program does not specifically output to a file. Upon execution you will need to redirect stdout.
 - in order to change the read limit, open readersAndWriters.h and change the global variable
