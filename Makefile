GCC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lm -g

BIN_DIR = bin
SRC_DIR = src

BIN = $(BIN_DIR)/main
SRC = $(wildcard $(SRC_DIR)/*.c)

all: $(BIN)

$(BIN): $(SRC)
	$(GCC) $(SRC) $(CFLAGS) -o $(BIN)

run: all
	./$(BIN)

debug: all
	gdb ./$(BIN)