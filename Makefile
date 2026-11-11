CC = gcc
CFLAGS = -Wall -Wextra -std=c11

OBJ = main.o comment_removal.o file_inclusion.o macro_handler.o

mypreprocessor: $(OBJ)
	$(CC) $(OBJ) -o mypreprocessor

main.o: main.c preprocessor.h
	$(CC) $(CFLAGS) -c main.c

comment_removal.o: comment_removal.c preprocessor.h
	$(CC) $(CFLAGS) -c comment_removal.c

file_inclusion.o: file_inclusion.c preprocessor.h
	$(CC) $(CFLAGS) -c file_inclusion.c

macro_handler.o: macro_handler.c preprocessor.h
	$(CC) $(CFLAGS) -c macro_handler.c

clean:
	rm -f *.o mypreprocessor *.i temp1.txt temp2.txt
