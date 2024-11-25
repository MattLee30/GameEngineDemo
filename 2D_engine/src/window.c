// window.c

#include "window.h"

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

void renderWindow(GLFWwindow* window, void (*renderFunction)(void)) {
    glfwMakeContextCurrent(window);

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (renderFunction != NULL) {
        renderFunction();
    }

    glfwSwapBuffers(window);
}
