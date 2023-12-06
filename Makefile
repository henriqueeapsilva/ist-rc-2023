CC = gcc
CFLAGS = -Wall -pedantic

# Source files in the current directory
SRCS := $(wildcard User_Application/*.c)

# Source files in the 'src' directory
SRCS += $(wildcard protocol/*.c)

# Source files in the 'src' directory
#SRCS += $(wildcard ../AS_server/*.c)

# Object files corresponding to source files
OBJS := $(SRCS:.c=.o)

TARGET = user

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
