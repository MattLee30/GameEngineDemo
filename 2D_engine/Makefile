APP_NAME = sharkitect
SRC_DIR = ./src
BUILD_DIR = ./build
C_FILES = $(wildcard $(SRC_DIR)/*.c)

APP_DEFINES := 
APP_INCLUDES := -I/opt/homebrew/include
APP_LINKERS := -framework Cocoa -framework OpenGL -framework IOKit -L/opt/homebrew/lib -lglfw

# Compile target
compile: $(BUILD_DIR)
	clang $(C_FILES) -o $(BUILD_DIR)/$(APP_NAME) $(APP_INCLUDES) $(APP_LINKERS)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up the build directory
clean:
	rm -rf $(BUILD_DIR)
