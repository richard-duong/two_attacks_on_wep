CC=gcc
CFLAGS= -lcrypto

all: packet test

packet:
	$(CC) -o packet packet.c $(CFLAGS)

test:
	$(CC) $(CFLAGS) -o test test.c
