SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/lab1/*.c)
SRC_FILES_2 := $(wildcard $(SRC_DIR)/lab2/*.c)
TOP_DIR := .
CFLAGS := -g -std=c99 -O0 -Wall -Werror -Wpedantic -Wno-deprecated-declarations
LDFLAGS := -I$(SRC_DIR)/
CC := gcc

all: lab2
	mkdir -p $(TOP_DIR)/out
	for file in $(SRC_FILES); do $(CC) $(CFLAGS) $(LDFLAGS) $$file -o $(TOP_DIR)/out/`basename $$file .c`; done

clean:
	rm -r $(TOP_DIR)/out

lab2:
	mkdir -p $(TOP_DIR)/out
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_1.c -o $(TOP_DIR)/out/lab2_1
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_2.c -o $(TOP_DIR)/out/lab2_2
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_3.c -o $(TOP_DIR)/out/lab2_3
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_4.c -o $(TOP_DIR)/out/lab2_4
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_5.c -o $(TOP_DIR)/out/lab2_5
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_6.c -o $(TOP_DIR)/out/lab2_6
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_7.c -o $(TOP_DIR)/out/lab2_7
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_8.c -o $(TOP_DIR)/out/lab2_8
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_8.c -DMODE=1 -o $(TOP_DIR)/out/lab2_8_1
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_9.c -o $(TOP_DIR)/out/lab2_9
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_9.c -DMODE=1 -o $(TOP_DIR)/out/lab2_9_1
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_10.c -o $(TOP_DIR)/out/lab2_10
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_10_1.c -o $(TOP_DIR)/out/lab2_10_1
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab2/lab2_11.c -o $(TOP_DIR)/out/lab2_11

