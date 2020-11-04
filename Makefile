CC=gcc
CFLAGS= -lcrypto
HEADERS= 	header/iv.h \
					header/ip_header.h \
					header/crc32.h \
					header/packet.h \
					header/rc4.h

all: program tests

program:
	$(CC) -o outputs/program.out program.c $(CFLAGS)

tests: crc32_test packet_test

crc32_test:
	$(CC) -o outputs/crc32_test.out tests/crc32_test.c $(CFLAGS)

packet_test:
	$(CC) -o outputs/packet_test.out tests/packet_test.c $(CFLAGS)

clean:
	rm outputs/*
