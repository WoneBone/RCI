CC=gcc
CFLAGS=-g -Wall


COR: main.o connect.o com.o what_in.o tp.o
	$(CC) $(CFLAGS) -o COR main.o connect.o com.o what_in.o tp.o

main.o: main.c header.h
	$(CC) -c $(CFLAGS) main.c
connect.o: connect.c header.h
	$(CC) -c $(CFLAGS) connect.c
com.o: com.c header.h
	$(CC) -c $(CFLAGS) com.c 
what_in.o: what_in.c header.h
	$(CC) -c $(CFLAGS) what_in.c
tp.o: header.h tp.c
	$(CC) -c $(CFLAGS) tp.c
clean:
	rm  *.o a.out COR
