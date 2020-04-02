CC=gcc
CFLAGS= -g
LDFLAGS=
EXEC= Flow
SRC= $(wildcard src/*.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

Flow: $(OBJ)
	@$(CC) -o bin/$@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf obj/*.o

mrproper: clean
	@rm -rf $(EXEC)