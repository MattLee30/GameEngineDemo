// window.h

#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

GLFWwindow* initializeWindow(const char* title, int width, int height,
                             GLFWframebuffersizefun framebufferSizeCallback,
                             GLFWmousebuttonfun mouseButtonCallback,
                             GLFWcursorposfun cursorPosCallback);

#endif // WINDOW_H
