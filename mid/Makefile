
CC = gcc
CFLAGS = -Wall -std=c99
SRC = src/main.c src/lexer.c src/parser.c src/interpreter.c
OBJ = $(SRC:.c=.o)
EXEC = aura

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC) tests/run_lexer_test
