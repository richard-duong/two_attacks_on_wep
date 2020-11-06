CC=gcc
CFLAGS= -lcrypto
HEADERS= 	../header/iv.h \
					../header/ip_header.h \
					../header/crc32.h \
					../header/packet.h \
					../header/rc4.h

all: alice carolwep ap carol bob

alice:
	$(CC) alice.c -o alice.out $(CFLAGS)

carolwep:
	$(CC) carolwep.c -o carolwep.out $(CFLAGS)

ap:
	$(CC) ap.c -o ap.out $(CFLAGS)

carol:
	$(CC) carol.c -o carol.out $(CFLAGS)

bob:
	$(CC) bob.c -o bob.out $(CFLAGS)



clean:
	rm *.out
