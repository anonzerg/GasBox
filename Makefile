CC := gcc
CFLAGS := -Wall -Wextra -std=c99
LIBS := -lraylib -lgsl -lcblas -lm
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=build/%.o)
TARGET := gas

build/$(TARGET): $(OBJS) | build
	$(CC) $^ -o $@ $(LIBS)

build/%.o: src/%.c | build
	$(CC) -c $< -o $@ $(CFLAGS)

build:
	mkdir build

clean:
	rm -rf build/*
