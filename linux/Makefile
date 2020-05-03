CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -g 
LDFLAGS = 'sdl2-config --cflags --libs'
EXEC = ./src/traverse


all :$(EXEC)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@  $^ $(LDFLAGS) $(CFLAGS)

clean :
	rm -rf src/*.o

cleanall : clean
	rm -rf $(EXEC)
