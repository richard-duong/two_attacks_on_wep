CC=gcc
CFLAGS= -lcrypto
HEADERS= 	../header/iv.h \
					../header/ip_header.h \
					../header/crc32.h \
					../header/packet.h \
					../header/rc4.h

all: clean alice carolwep

alice:
	$(CC) alice.c -o alice.out $(CFLAGS)

carolwep:
	$(CC) carolwep.c -o carolwep.out $(CFLAGS)

clean:
	rm *.out
