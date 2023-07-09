CC = gcc
CPP = g++
SRCS = $(shell find ./src -name "*.c")
HDRS = $(shell find ./src/include -name "*.h")
OBJS = $(patsubst %.c, %.o, $(SRCS))
FLAGS = -O3 -ftree-vectorize
CPPFLAGS = -std=c++17  -O3 -Wall -Werror
TARGET = testing

all: testing

main: $(OBJS)
	$(CC) $(FLAGS) main.c -o $(TARGET) $(OBJS)

testing: $(OBJS)
	$(CPP) $(CPPFLAGS) main.cpp -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

format:
	clang-format -i -style="file:clang-format.txt" src/*.c src/include/*.h

clean: 
	rm  $(TARGET) $(TESTING_TARGET) $(OBJS)
