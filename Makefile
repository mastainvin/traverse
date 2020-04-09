CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall
LDFLAGS = -lSDL2 -lSDL2_Image -lSDL2_ttf -lm
EXEC = ./bin/traverse


all :$(EXEC)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@  $^ $(LDFLAGS) $(CFLAGS)

clean :
	rm -rf src/*.o

cleanall : clean
	rm -rf $(EXEC)
