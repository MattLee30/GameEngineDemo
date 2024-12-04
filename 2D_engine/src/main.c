#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "headers/window.h"
#include "headers/framebuffer.h"
#include "headers/structs.h"
#include "headers/gameobject.h"
#include "headers/input.h"
#include "headers/drawShape.h"
#include "headers/circle.h"
#include "headers/square.h"


int main() {
    if (!glfwInit()) {
        return -1;
    }

    // Create a single window
    GLFWwindow* window = initializeWindow("GameObject Demo", 800, 600,
                                          framebuffer_size_callback,
                                          mouse_button_callback,
                                          cursor_position_callback);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Setup window context
    glfwMakeContextCurrent(window);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    framebuffer_size_callback(window, fbWidth, fbHeight);

    // Time variables
    float lastTime = glfwGetTime();

    // Initialize game objects
    initializeBalls(10);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Calculate time delta
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Get framebuffer size in each loop (in case of window resize)
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        // Render the selection bar
        renderSelectionBar(fbWidth, fbHeight);

        // Render the game area
        renderGameArea(fbWidth, fbHeight, deltaTime);

        glfwSwapBuffers(window);
    }

    // Cleanup
    free(gameObjects);
    gameObjects = NULL;

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
