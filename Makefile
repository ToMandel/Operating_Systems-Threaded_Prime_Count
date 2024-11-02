CC=gcc
OBJS= nicecmp.o
EXEC=prog
DEBUG= -g
CFLAGS=-std=c99 -Wall -Werror $(DEBUG)

$(EXEC) : $(OBJS)
	$(CC) nicecmp.o -o nicecmp

nicecmp.o: nicecmp.c

clean:
	rm -f $(OBJS) $(EXEC)
