CC=gcc
CFLAGS=-lreadline -g -Wall

all: clean main

main: main.c
	$(CC) $(CFLAGS) -o main main.c `sdl2-config --cflags --libs`

clean:
	$(RM) -rf main *.dSYM
