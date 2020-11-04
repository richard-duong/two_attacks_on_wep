CC=gcc
CFLAGS= -lcrypto
HEADER= header/iv.h \
				header/ip_header.h \
				header/crc32.h \
				header/packet.h \
				header/rc4.h

all: program tests

program:
	$(CC) -o outputs/program.out program.c $(CFLAGS)

tests: crc32_test

crc32_test:
	$(CC) -o outputs/crc32_test tests/crc32_test.c $(CFLAGS)

clean:
	rm output/*.out
