CC = gcc
CFLAGS = -Wall -Wextra \
	-Wno-return-type \
	-Wno-discarded-qualifiers

BUILD_DIR = build

str: str.c ../str.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -ggdb str.c -o $(BUILD_DIR)/str \
		&& ./$(BUILD_DIR)/str

vec: vec.c ../vec.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -ggdb vec.c -o $(BUILD_DIR)/vec \
		&& ./$(BUILD_DIR)/vec

tree: tree.c ../tree.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -ggdb tree.c -o $(BUILD_DIR)/tree \
		&& ./$(BUILD_DIR)/tree

clean:
	rm -rf $(BUILD_DIR)/*
