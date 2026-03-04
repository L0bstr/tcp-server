CC = gcc
CFLAGS = -I./include

all:
	$(CC) $(CFLAGS) -o ./build/server src/main.c src/lib.c

bear:
	mkdir -p build
	bear --output ./build/compile_commands.json -- make
