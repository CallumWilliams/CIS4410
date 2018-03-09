Name: Callum Williams
Date: February 26th, 2018
For: CIS4410

Compile:
	make
	('make server' to just compile the server)
	('make client' to just compule the client)

Run:
	To run the server:
		./server <number-of-groups>
	To run the client:
		./client (<number-of-readers>-<number-of-writers>) x number of groups
		Example: to run two groups, one with 5 readers and 2 writers, and one with 3 readers and 3 writers:
		./client 5-2 3-3

Clean:
	make clean

