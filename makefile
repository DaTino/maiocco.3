CC=gcc
CFLAGS=-I.

all: oss user

oss: oss.o
	$(CC) -o oss oss.o

user: user.o
	$(CC) -o user user.o

clean:
	rm oss user oss.o user.o
