OBJ = main.o
CFLAGS = -Wall -g
CC = gcc

main:$(OBJ)
		$(CC) $(CFLAGS) -o $@ $(OBJ)

clean:
		rm -f *.o main

