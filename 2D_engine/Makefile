APP_NAME = myApp
BUILD_DIR = ./build
SRC_DIR = ./src/
C_FILES = $(wildcard $(SRC_DIR)*.c)

APP_DEFINES := 
APP_INCLUDES := -I/opt/homebrew/include -framework Cocoa -framework OpenGL -framework IOKit
APP_LINKERS := -L/opt/homebrew/lib -lglfw

# Compile target
compile: $(BUILD_DIR)
	clang $(C_FILES) -o $(BUILD_DIR)/$(APP_NAME) $(APP_INCLUDES) $(APP_LINKERS)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up the build directory
clean:
	rm -rf $(BUILD_DIR)
