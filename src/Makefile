CC = gcc
SRC = $(wildcard src/*.c)

 

ifeq ($(OS),Windows_NT)
    OBJ = $(SRC:.c=.o)
    LDFLAGS = -L lib -lmingw32 -lSDL2main -lSDL2
    CFLAGS = -I include
    EXEC = traverse.exe
else
    OBJ = $(SRC:.c=.o)
    LDFLAGS = -lSDL2 -lSDL2_Image -lSDL2_ttf -lm
    EXEC = traverse
endif

 

all :$(EXEC)

 

%.o : %.c
    $(CC) $(CFLAGS) -o $@ -c $<

 

$(EXEC): $(OBJ)
    $(CC) -o $@ $^ $(LDFLAGS)
