CC=gcc
CFLAGS=-O3 -Wall -std=c99 


COR: main.o connect.o
	$(CC) $(CFLAGS) -o COR main.o connect.o

main.o: main.c header.h
	$(CC) -c $(CFLAGS) main.c
connect.o: connect.c header.h
	$(CC) -c $(CFLAGS) connect.c
clean::
	rm  *.o a.out COR
