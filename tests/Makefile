CC = gcc
CFLAGS = -Wall -Wextra -Wno-return-type -Wno-discarded-qualifiers

TESTS = $(wildcard *.c)

BUILD_DIR = build

str: str.c ../str.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -ggdb str.c -o $(BUILD_DIR)/str
	./$(BUILD_DIR)/str

clean:
	rm -rf $(BUILD_DIR)/*
