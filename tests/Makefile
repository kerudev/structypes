CC = gcc
CFLAGS = -std=c99 \
	-fsanitize=address -ggdb \
	-Wall -Wextra \
	-Wno-return-type \
	-Wno-discarded-qualifiers

BUILD_DIR = build

TESTS = $(wildcard *.c)
NAMES = $(patsubst %.c, %, $(TESTS))

.PHONY: all clean $(NAMES)

all: $(NAMES)

$(BUILD_DIR):
	@mkdir -p $@

$(NAMES): %: $(BUILD_DIR)
	$(CC) $(CFLAGS) $@.c -o $(BUILD_DIR)/$@ && ./$(BUILD_DIR)/$@

clean:
	rm -rf $(BUILD_DIR)/*
