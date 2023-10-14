CC = gcc
CFLAGS = -g -Wall
LIBS = -lcrypto -lssl

all: hashCompare

hashCompare: main.o hash.o t3c_struct.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

main.o: main.c hash.h t3c_struct.h
	$(CC) $(CFLAGS) -c $<

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c $<

t3c_struct.o: t3c_struct.c t3c_struct.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o hashCompare

