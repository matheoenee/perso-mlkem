# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Output executable name
TARGET = main

# List of source files
SRCS = main.c utils.c

# Default rule to build the project
all: $(TARGET)

# Link the source files directly to create the executable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Clean rule to remove generated files
clean:
	rm -f $(TARGET)
