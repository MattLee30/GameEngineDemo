# Variables
APP_NAME = myApps
BUILD_DIR = ./build
SRC_DIR = ./src
HEADER_DIR = ./src/headers
INCLUDE_PATHS = -I/opt/homebrew/include -I$(HEADER_DIR) -I/opt/homebrew/Cellar/freetype/2.13.3/include/freetype2
LIBRARY_PATHS = -L/opt/homebrew/lib
LIBRARIES = -lglfw -lfreetype -lm
FRAMEWORKS = -framework Cocoa -framework OpenGL -framework IOKit

# Gather all source files
C_FILES = $(wildcard $(SRC_DIR)/*.c)

# Object files will be stored in the build directory
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target to build the application
all: $(BUILD_DIR)/$(APP_NAME)

# Compile each C source file into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	clang -c $< -o $@ $(INCLUDE_PATHS)

# Link object files to create the executable
$(BUILD_DIR)/$(APP_NAME): $(OBJ_FILES)
	clang $(OBJ_FILES) -o $@ $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LIBRARIES) $(FRAMEWORKS)

# Ensure the build directory exists before compiling
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
