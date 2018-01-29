all: rAndW
	gcc -pthread readersAndWriters.o -o readersAndWriters

rAndW: readersAndWriters.c readersAndWriters.h
	gcc -pthread -c readersAndWriters.c -o readersAndWriters.o

clean:
	rm readersAndWriters
	rm *.o
