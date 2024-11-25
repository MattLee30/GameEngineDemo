// framebuffer.c

#include "headders/framebuffer.h"

void framebuffer_size_callback(GLFWwindow* window, int fbWidth, int fbHeight) {
    glfwMakeContextCurrent(window);

    glViewport(0, 0, fbWidth, fbHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = (float)fbWidth / (float)fbHeight;

    if (aspectRatio >= 1.0f) {
        glOrtho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, -1.0f, 1.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
