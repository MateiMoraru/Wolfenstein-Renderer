GCC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Iinclude -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -lm -g

WINGCC = x86_64-w64-mingw32-gcc
SDL2_WIN = /home/matei/Downloads/SDL2-win/SDL2

WINCFLAGS = -std=c99 -Wall -Wextra -Iinclude -I$(SDL2_WIN)/include -D_REENTRANT -DSDL_MAIN_HANDLED
WINLDFLAGS = -L$(SDL2_WIN)/lib/x86_64-w64-mingw32 -lmingw32 -lSDL2main -lSDL2 -mwindows -static -static-libgcc -static-libstdc++

BIN_DIR = bin
WIN_BIN_DIR = win-build
SRC_DIR = src

BIN = $(BIN_DIR)/main
WIN_BIN = $(WIN_BIN_DIR)/main.exe
SRC = $(wildcard $(SRC_DIR)/*.c)

all: $(BIN)

$(BIN): $(SRC)
	$(GCC) $(SRC) $(CFLAGS) -o $(BIN)

run: all
	./$(BIN)

debug: all
	gdb ./$(BIN)

$(WIN_BIN_DIR):
	mkdir -p $(WIN_BIN_DIR)

win: $(WIN_BIN_DIR)
	$(WINGCC) $(SRC) $(WINCFLAGS) $(WINLDFLAGS) -o $(WIN_BIN)