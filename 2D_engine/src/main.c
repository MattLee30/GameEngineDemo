#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "headders/window.h"
#include "headders/framebuffer.h"
#include "headders/structs.h"
#include "headders/gameobject.h"
#include "headders/input.h"
#include "headders/drawShape.h"
#include "headders/circle.h"
#include "headders/square.h"


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
