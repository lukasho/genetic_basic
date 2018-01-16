CC=gcc
CFLAGS=-Wextra -Wall -Werror -W -std=c99

start: start.c start.h fitness.c 
	$(CC) $(CFLAGS) start.c fitness.c -o start