# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Output folder for intermediate files
BUILD_DIR = build

# Source files
SRC = main.c pieces.c board.c

# Object files
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

# Executable file
TARGET = tetris

# Commands
MKDIR_P = mkdir -p
RM = rm -rf

# Default target
all: $(TARGET)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	@$(MKDIR_P) $(BUILD_DIR)

# Link object files into the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

# Compile source files into object files in the build directory
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build directory and executable
clean:
	$(RM) $(BUILD_DIR) $(TARGET)

.PHONY: all clean
