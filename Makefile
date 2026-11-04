CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = build/polisz
SRCS = src/main.c src/lexer.c src/parser.c src/interpreter.c
OBJS = $(SRCS:src/%.c=build/%.o)
HEADERS = src/lexer.h src/parser.h src/interpreter.h
EXAMPLES = $(wildcard examples/*.pol)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(EXAMPLES): $(TARGET)
	./$(TARGET) "$@"

build/%.o: src/%.c $(HEADERS) | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean $(EXAMPLES)
