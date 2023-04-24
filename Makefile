CC=gcc
CFLAGS=-std=gnu99 -pthread -Wall -Werror -Wextra

proj2: main.o arg_line.o
	$(CC) $(CFLAGS) -o proj2 main.o arg_line.o

main.o: main.c arg_line.h
	$(CC) $(CFLAGS) -c main.c

arg_line.o: arg_line.c arg_line.h
	$(CC) $(CFLAGS) -c arg_line.c

clean:
	rm -f *.o proj2
