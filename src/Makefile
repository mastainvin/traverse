CC = gcc
EXEC = traverse
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all :$(EXEC)

%.o : %.c
	$(CC) -o $@ -c $<

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ -lSDL2 -lSDL2_Image -lSDL2_ttf -lm


clean :
	rm -rf *.o
