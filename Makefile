SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/lab1/*.c)
SRC_FILES_2 := $(wildcard $(SRC_DIR)/lab2/*.c)
TOP_DIR := .
CFLAGS := -g -std=c99 -O0 -Wall -Werror -Wpedantic -Wno-deprecated-declarations
LDFLAGS := -I$(SRC_DIR)/
CC := gcc

clean:
	rm -r $(TOP_DIR)/out

lab2:
	mkdir -p $(TOP_DIR)/out
	for file in $(SRC_FILES_2); do $(CC) $(CFLAGS) $(LDFLAGS) $$file -o $(TOP_DIR)/out/`basename $$file .c`; done

all:
	mkdir -p $(TOP_DIR)/out
	for file in $(SRC_FILES); do $(CC) $(CFLAGS) $(LDFLAGS) $$file -o $(TOP_DIR)/out/`basename $$file .c`; done
