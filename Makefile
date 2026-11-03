CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = polisz
SRCS = main.c lexer.c parser.c interpreter.c
OBJS = $(SRCS:.c=.o)
HEADERS = lexer.h parser.h interpreter.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

parser.o: parser.c parser.h lexer.h
	$(CC) $(CFLAGS) -c parser.c

interpreter.o: interpreter.c interpreter.h parser.h lexer.h
	$(CC) $(CFLAGS) -c interpreter.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
