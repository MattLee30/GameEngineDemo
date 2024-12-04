// window.c

#include "headders/window.h"

GLFWwindow* initializeWindow(const char* title, int width, int height,
                             GLFWframebuffersizefun framebufferSizeCallback,
                             GLFWmousebuttonfun mouseButtonCallback,
                             GLFWcursorposfun cursorPosCallback) {
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (framebufferSizeCallback != NULL) {
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        // Initial call to set up the viewport
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        framebufferSizeCallback(window, fbWidth, fbHeight);
    }

    if (mouseButtonCallback != NULL) {
        glfwSetMouseButtonCallback(window, mouseButtonCallback);
    }

    if (cursorPosCallback != NULL) {
        glfwSetCursorPosCallback(window, cursorPosCallback);
    }

    return window;
}
