CC=gcc
CFLAGS= -lcrypto

all: packet tests

packet:
	$(CC) -o outputs/packet.out packet.c $(CFLAGS)

tests: crc32_test

crc32_test:
	$(CC) -o outputs/crc32_test tests/crc32_test.c $(CFLAGS)

clean:
	rm *.out
