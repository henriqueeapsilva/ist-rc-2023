CC = gcc
CFLAGS = -Wall -pedantic

SRCS = $(wildcard *.c)

TARGET = user

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
