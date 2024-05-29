# Compiler
CC = gcc

# Compiler flags
CFLAGS = -I./inc -Wall -Wextra -Werror

# Directories
SRCDIR = src
INCDIR = inc
OBJDIR = obj
BINDIR = bin

# Source files
SRCS = $(wildcard $(SRCDIR)/*.c)

# Object files
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Executable file
TARGET = $(BINDIR)/cpu_scheduler

# Default target
all: $(TARGET)

# Create directories if they do not exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

# Build target
$(TARGET): $(OBJDIR) $(BINDIR) $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
