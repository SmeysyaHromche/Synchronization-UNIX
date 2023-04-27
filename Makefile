CC=gcc
CFLAGS=-std=gnu99 -pthread -Wall -Werror -Wextra

proj2: main.o utils.o shared_memory.o proces_and_sem.o
	$(CC) $(CFLAGS) -o proj2 main.o utils.o shared_memory.o proces_and_sem.o

main.o: main.c proj2.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c proj2.h
	$(CC) $(CFLAGS) -c utils.c

shared_memory.o: shared_memory.c proj2.h
	$(CC) $(CFLAGS) -c shared_memory.c

proces_and_sem.o: proces_and_sem.c proj2.h
	$(CC) $(CFLAGS) -c proces_and_sem.c

clean:
	rm -f *.o proj2 proj2.out
