CC=gcc
CFLAGS= -lcrypto
HEADERS= 	header/iv.h \
					header/ip_header.h \
					header/crc32.h \
					header/packet.h \
					header/rc4.h

all: sockets

sockets: alice carolwep ap carol bob

alice:
	$(CC) -o outputs/alice.out src/alice.c $(CFLAGS)

carolwep:
	$(CC) -o outputs/carolwep.out src/carolwep.c $(CFLAGS)

ap:
	$(CC) -o outputs/ap.out src/ap.c $(CFLAGS)

carol:
	$(CC) -o outputs/carol.out src/carol.c $(CFLAGS)

bob:
	$(CC) -o outputs/bob.out src/bob.c $(CFLAGS)

tests: crc32_test packet_test rc4_test edit_test

crc32_test:
	$(CC) -o outputs/crc32_test.out tests/crc32_test.c $(CFLAGS)

packet_test:
	$(CC) -o outputs/packet_test.out tests/packet_test.c $(CFLAGS)

rc4_test:
	$(CC) -o outputs/rc4_test.out tests/rc4_test.c $(CFLAGS)

edit_test:
	$(CC) -o outputs/edit_test.out tests/edit_test.c $(CFLAGS)

clean:
	rm outputs/*
