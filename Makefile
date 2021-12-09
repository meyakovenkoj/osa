SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/lab1/*.c)
SRC_FILES_2 := $(wildcard $(SRC_DIR)/lab2/*.c)
TOP_DIR := .
CFLAGS := -g -std=gnu99 -O0 -Wall -Werror -Wpedantic -Wno-deprecated-declarations
LDFLAGS := -I$(SRC_DIR)/
CC := gcc

all: lab2 lab3 lab4 lab5
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

lab3:
	mkdir -p $(TOP_DIR)/out
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_1.c -o $(TOP_DIR)/out/lab3_1
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_2.c -o $(TOP_DIR)/out/lab3_2
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_3.c -o $(TOP_DIR)/out/lab3_3
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_4.c -o $(TOP_DIR)/out/lab3_4
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_5.c -o $(TOP_DIR)/out/lab3_5
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_6.c -o $(TOP_DIR)/out/lab3_6
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_7.c -o $(TOP_DIR)/out/lab3_7
	$(CC) $(CFLAGS) $(LDFLAGS) -DTASK5 $(SRC_DIR)/lab3/lab3_7.c -o $(TOP_DIR)/out/lab3_7_5
	$(CC) $(CFLAGS) $(LDFLAGS) -DTASK6 $(SRC_DIR)/lab3/lab3_7.c -o $(TOP_DIR)/out/lab3_7_6
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_8.c -o $(TOP_DIR)/out/lab3_8
	$(CC) $(CFLAGS) $(LDFLAGS) -DWITHSIG $(SRC_DIR)/lab3/lab3_9.c -o $(TOP_DIR)/out/lab3_9_1
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_9.c -o $(TOP_DIR)/out/lab3_9
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_10.c -o $(TOP_DIR)/out/lab3_10
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab3/lab3_11.c -o $(TOP_DIR)/out/lab3_11

lab4:
	mkdir -p $(TOP_DIR)/out
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_1.c -o $(TOP_DIR)/out/lab4_1
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_2.c -o $(TOP_DIR)/out/lab4_2
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_3.c -o $(TOP_DIR)/out/lab4_3
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_4.c -o $(TOP_DIR)/out/lab4_4
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_4e.c -o $(TOP_DIR)/out/lab4_4e
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_5.c -o $(TOP_DIR)/out/lab4_5
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_6.c -o $(TOP_DIR)/out/lab4_6
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_6e.c -o $(TOP_DIR)/out/lab4_6e
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_7.c -o $(TOP_DIR)/out/lab4_7
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_8.c -o $(TOP_DIR)/out/lab4_8
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_9.c -o $(TOP_DIR)/out/lab4_9
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_10.c -o $(TOP_DIR)/out/lab4_10
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab4/lab4_11.c -o $(TOP_DIR)/out/lab4_11

lab5:
	mkdir -p $(TOP_DIR)/out
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_1.c -o $(TOP_DIR)/out/lab5_1
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_2.c -o $(TOP_DIR)/out/lab5_2
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_3.c -o $(TOP_DIR)/out/lab5_3
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_4.c -o $(TOP_DIR)/out/lab5_4
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_5.c -o $(TOP_DIR)/out/lab5_5
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_6.c -o $(TOP_DIR)/out/lab5_6
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_6client.c -o $(TOP_DIR)/out/lab5_6client
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_7.c -o $(TOP_DIR)/out/lab5_7
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_7client.c -o $(TOP_DIR)/out/lab5_7client
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_8.c -o $(TOP_DIR)/out/lab5_8
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_9.c -o $(TOP_DIR)/out/lab5_9
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_9client.c -o $(TOP_DIR)/out/lab5_9client
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_10.c -o $(TOP_DIR)/out/lab5_10
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_10client.c -o $(TOP_DIR)/out/lab5_10client
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DIR)/lab5/lab5_11.c -o $(TOP_DIR)/out/lab5_11

