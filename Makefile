CC=gcc
CFLAGS=-std=c99
SRC=prod.c
OBJ=prod.o
BIN=prod

all:$(OBJ)
	$(CC) -o $(BIN) $(OBJ)

%.o:%.c
	$(CC) $(CFLAGS) -c $^