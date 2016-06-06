CC=gcc
FLAGS=-Wall -O3

all: main

main: main.o general.o network.o
	$(CC) $(FLAGS) *.o -o araneus

main.o: src/main.*
	$(CC) $(FLAGS) -c src/main.c

general.o: src/general.*
	$(CC) $(FLAGS) -c src/general.c

network.o: src/network.*
	$(CC) $(FLAGS) -c src/network.c

clean:
	rm *.o araneus
