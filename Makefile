# Makefile for compiling and running matrix multiplication tests

CC = gcc
CFLAGS = -lpthread -lrt
TARGET = matrix_mult
SRC = test.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(CFLAGS)

clean:
	rm -f $(TARGET) results.csv

run: $(TARGET)
	./run_tests.sh

