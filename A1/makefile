all: rAndW
	gcc readersAndWriters.o -o readersAndWriters

rAndW: readersAndWriters.c
	gcc -c readersAndWriters.c -o readersAndWriters.o

clean:
	rm readersAndWriters
	rm *.o
