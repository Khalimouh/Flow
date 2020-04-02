CC=gcc
CFLAGS= -g -W -Wall 
LDFLAGS= -lm
EXEC= Flow
SRC= $(wildcard src/*.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

Flow: $(OBJ)
	@$(CC) -o bin/$@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper 

run:
	@cd bin/
	@./Flow ../test 8

clean:
	@rm -rf obj/*.o

mrproper: clean
	@rm -rf $(EXEC)