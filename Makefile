SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/lab1/*.c)
TOP_DIR := .
CFLAGS := -g -O0 -Werror -Wall -Wpedantic -Waddress -Wbool-operation -Wchar-subscripts -Wcomment -Wint-in-bool-context -Winit-self -Wlogical-not-parentheses -Wuninitialized -Wvec-elem-size -Wmemset-transposed-args -Wmisleading-indentation -Wunused-macros -Wnonnull -Wparentheses -Wpessimizing-move -Wreorder -Wreturn-type -Wsequence-point -Wsign-compare -Wsizeof-pointer-div -Wsizeof-pointer-memaccess -Wstrict-aliasing -Wswitch -Wtautological-compare -Wuninitialized -Wunused-function -Wunused-variable -Wextra -Wempty-body -Wimplicit-fallthrough
LDFLAGS := -I$(SRC_DIR)/
CC := gcc

clean:
	rm -r $(TOP_DIR)/out

all:
	mkdir -p $(TOP_DIR)/out
	for file in $(SRC_FILES); do $(CC) $(CFLAGS) $(LDFLAGS) $$file -o $(TOP_DIR)/out/`basename $$file .c`; done
