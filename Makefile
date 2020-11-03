CC=gcc
CFLAGS= -lcrypto

all: packet test

packet:
	$(CC) -o packet.out packet.c $(CFLAGS)

test:
	$(CC) -o test.out test.c $(CFLAGS)

clean:
	rm *.out
