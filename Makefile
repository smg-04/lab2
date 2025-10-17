CC = gcc
CFLAGS = -Wall -fPIC -Iinclude

SRC_DIR = src
LIB_DIR = lib

SRC = calc.c
MAIN = calcmain.c
MAIN_DYNAMIC = main_dynamic.c

STATIC_LIB = $(LIB_DIR)/libcalc.a
SHARED_LIB = $(LIB_DIR)/libcalc.so

BIN_STATIC = main_static
BIN_SHARED = main_shared
BIN_DYNAMIC = main_dynamic

.PHONY: all clean

all: $(BIN_STATIC) $(BIN_SHARED) $(BIN_DYNAMIC)

calc.o: $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(STATIC_LIB): calc.o | $(LIB_DIR)
	ar rcs $@ calc.o

$(SHARED_LIB): calc.o | $(LIB_DIR)
	$(CC) -shared -o $@ calc.o

$(BIN_STATIC): $(MAIN) $(STATIC_LIB)
	$(CC) $(MAIN) -L$(LIB_DIR) -lcalc -o $@

$(BIN_SHARED): $(MAIN) $(SHARED_LIB)
	$(CC) $(MAIN) -L$(LIB_DIR) -lcalc -o $@

$(BIN_DYNAMIC): $(MAIN_DYNAMIC) $(SHARED_LIB)
	$(CC) $(MAIN_DYNAMIC) -ldl -o $@

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

clean:
	rm -f *.o $(LIB_DIR)/* $(BIN_STATIC) $(BIN_SHARED) $(BIN_DYNAMIC)
