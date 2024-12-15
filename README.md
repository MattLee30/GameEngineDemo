# Sharkitect GameEngineDemo

An independent study project conducted by Grayson von Goetz und Schwanenfliess and Matthew Lee, showcasing a physics game engine written and compiled in C that utilizes vector math to calculate collisions which take into account user interaction.

When running Sharkitect, use the green triangle to toggle play and pause, hit the red button in the upper right corner to clear all objects, and use the triangles and squares on the left to select which shape you want to make by clicking in the window.


## Instructions

To compile and run the project, follow these steps:

1. **Install dependencies:**

   Before compiling, make sure you have the required packages installed:

   - **Homebrew** (for package management):  
     If you don't have Homebrew installed, you can install it by running:
     ```bash
     /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
     ```

   - **OpenGL**:  
     Install OpenGL using Homebrew:
     ```bash
     brew install opengl
     ```

2. **Navigate to the `2D_Engine` folder in your directory:**
   ```bash
   cd 2D_Engine
   
3. **Compile the project using the `make` command:**
   ```bash
   make compile
   ```

> **Note:** Ensure that you have all the necessary build tools installed before attempting to compile the project.

   
3. **Run the compiled application:**
   ```bash
   ./build/sharkitect
   ```
