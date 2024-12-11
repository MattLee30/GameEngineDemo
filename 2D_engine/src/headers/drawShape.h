// drawShape.h

#ifndef DRAWSHAPE_H
#define DRAWSHAPE_H

#include <GLFW/glfw3.h>

void renderSelectionBar(GLFWwindow* window, int fbWidth, int fbHeight);
void renderTopBar(GLFWwindow* window, int fbWidth, int fbHeight);
void renderGameArea(int fbWidth, int fbHeight, float deltaTime);
void drawCircle(float x, float y, float radius, int segments);
void drawSquare(float x, float y, float size);

#endif // DRAWSHAPE_H
