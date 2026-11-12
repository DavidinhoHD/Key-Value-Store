CC     = gcc-16
CFLAGS = -std=c23 -pedantic -Wall -Wextra -Iinc
SRCS   = $(wildcard src/*.c)
HDRS = $(wildcard inc/*.h)

bin/main: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(SRCS) -o bin/main

clean:
	rm -f bin/main