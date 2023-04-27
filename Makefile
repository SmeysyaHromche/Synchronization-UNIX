CC=gcc
CFLAGS=-std=gnu99 -pthread -Wall -Werror -Wextra

proj2: main.o helper_main.o shared_memory.o proces_and_sem.o
	$(CC) $(CFLAGS) -o proj2 main.o helper_main.o shared_memory.o proces_and_sem.o

main.o: main.c helper_main.h
	$(CC) $(CFLAGS) -c main.c

helper_main.o: helper_main.c helper_main.h
	$(CC) $(CFLAGS) -c helper_main.c

shared_memory.o: shared_memory.c helper_main.h
	$(CC) $(CFLAGS) -c shared_memory.c

proces_and_sem.o: proces_and_sem.c helper_main.h
	$(CC) $(CFLAGS) -c proces_and_sem.c

clean:
	rm -f *.o proj2 proj2.out
