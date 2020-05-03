CC = gcc
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
CPPFLAGS = -I include
CFLAGS = -g
LDFLAGS = -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image
EXEC = src\traverse.exe

all :$(EXEC)

%.o : %.c
	$(CC) -o $@ -c $< $(CPPFLAGS) $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@  $^ $(LDFLAGS) $(CFLAGS)


clean:
	del src\*.o

cleanall: clean
	del $(EXEC)
