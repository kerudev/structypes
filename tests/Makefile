CC = gcc
CFLAGS = -Wall -Wextra

TESTS = $(wildcard *.c)
SOURCE = $(wildcard ../*.h)

BUILD_DIR = build

all: $(BIN)

$(BIN): $(TESTS) $(SOURCE)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o 

str: str.c ../str.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) str.c ../str.h -o $(BUILD_DIR)/str
	./$(BUILD_DIR)/str

clean:
	rm -rf $(BUILD_DIR)/*
