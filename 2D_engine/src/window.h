// window.h

#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

// Function to initialize a GLFW window with specified callbacks
GLFWwindow* initializeWindow(const char* title, int width, int height,
                             GLFWframebuffersizefun framebufferSizeCallback,
                             GLFWmousebuttonfun mouseButtonCallback,
                             GLFWcursorposfun cursorPosCallback);

// Function to render content in the window
void renderWindow(GLFWwindow* window, void (*renderFunction)(void));

#endif // WINDOW_H
