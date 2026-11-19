CC = gcc
CFLAGS = -Wall -Wextra -g

all: msh

msh: msh.c
	$(CC) $(CFLAGS) -o msh msh.c

clean:
	rm -f msh
