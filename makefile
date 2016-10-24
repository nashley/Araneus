CC=gcc
FLAGS=-Wall -Og -g

all: main

main: main.o general.o network.o parse.o
	$(CC) $(FLAGS) *.o -o araneus

main.o: src/main.*
	$(CC) $(FLAGS) -c src/main.c

general.o: src/general.*
	$(CC) $(FLAGS) -c src/general.c

network.o: src/network.*
	$(CC) $(FLAGS) -c src/network.c

parse.o: src/main.*
	$(CC) $(FLAGS) -c src/parse.c

clean:
	rm *.o araneus
