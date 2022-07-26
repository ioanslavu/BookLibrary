CC=gcc
CFLAGS=-Wall -Wextra -std=c99

build: Tema2

Tema2: main.o book.o library.o user.o messages.o hashtable.o LinkedList.o
	$(CC) $(CFLAGS) main.o book.o library.o user.o messages.o hashtable.o LinkedList.o -o main

hashtable.o: hashtable.h hashtable.c
	$(CC) $(CFLAGS) hashtable.c -c -o hashtable.o

LinkedList.o: LinkedList.h LinkedList.c
	$(CC) $(CFLAGS) LinkedList.c -c -o LinkedList.o

messages.o: messages.h messages.c
	$(CC) $(CFLAGS) messages.c -c -o messages.o

book.o: book.h book.c
	$(CC) $(CFLAGS) book.c -c -o book.o

library.o: library.h library.c
	$(CC) $(CFLAGS) library.c -c -o library.o

user.o: user.h user.c
	$(CC) $(CFLAGS) user.c -c -o user.o

main.o: main.c 
	$(CC) $(CFLAGS) main.c -c -o main.o

clean:
	rm -f ./*.o
	rm main

