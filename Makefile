CC=gcc
OBJS=main.o 
CFLAGS=-Wall -Wextra -pedantic -std=c99 -lm -lraylib

game: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c

run: game
	./a.out

clean: 
	rm *.o 
