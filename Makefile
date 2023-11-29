CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRCS = $(wildcard *.c)

TARGET = user

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
