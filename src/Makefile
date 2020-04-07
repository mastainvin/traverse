CC = gcc
SRC = $(wildcard src/*.c)

ifeq ($(OS),Windows_NT)  
	OBJ = $(SRC:.c=.o + wildcard bin/main/*.c)
	LINK = -I include -L lib -lmingw32 -lSDL2main -lSDL2 
	EXEC = traverse.exe 
    else
    	OBJ = $(SRC:.c=.o)
	LINK = -lSDL2 -lSDL2_Image -lSDL2_ttf -lm
	EXEC = traverse
endif



all :$(EXEC)


%.o : %.c
	$(CC) -o $@ -c $<

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LINK)


clean :
	rm -rf *.o

