# Makefile for the text editor project

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Linker flags
LDFLAGS = -lncurses

# Source files
SRCS = main.c rope.c editor.c normal.c insert.c command.c search.c
# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = looed

# Default target
all: $(EXEC)

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(EXEC)

# Run the program
#run: all
#	./$(EXEC)

# Run the program with a specific file
#run-file: all
#	./$(EXEC) $<

# Help message
help:
	@echo "Usage:"
	@echo "  make all       - Compile the project"
	@echo "  make clean     - Remove object files and executable"
	@echo "  make run       - Run the program"
	@echo "  make run-file  - Run the program with a specific file"
	@echo "  make help      - Show this help message"