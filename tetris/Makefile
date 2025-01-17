# Compiler and flags
CC = gcc
CFLAGS = -Wall -I. -I./SDL/include  # Add SDL include path
LDFLAGS = -L -lSDL2               # Add SDL library path and link SDL2

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


# Compiler and flags
CC = gcc
CFLAGS = -Wall -I./drivers -I<SDL_INCLUDE_PATH>  # Add SDL include path
LDFLAGS = -L<SDL_LIB_PATH> -lSDL2               # Add SDL library path and link SDL2

# Output folder for intermediate files
BUILD_DIR = build

# Source files
SRC = main.c drivers/pieces.c drivers/board.c

# Object files
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

# Executable file
TARGET = tetris.exe

# Commands
MKDIR_P = mkdir
RM = del /Q
RMDIR = rmdir /S /Q

# Default target
all: $(TARGET)

# Create the necessary build directories
$(BUILD_DIR):
	@$(MKDIR_P) $(BUILD_DIR)
	@$(MKDIR_P) $(BUILD_DIR)/drivers

# Link object files into the executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# Compile source files into object files in the build directory
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@$(MKDIR_P) $(dir $@) # Ensure subdirectories exist
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build directory and executable
clean:
	$(RMDIR) $(BUILD_DIR)
	$(RM) $(TARGET)

.PHONY: all clean
