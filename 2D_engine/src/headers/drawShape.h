// drawShape.h

#ifndef DRAWSHAPE_H
#define DRAWSHAPE_H

#include <GLFW/glfw3.h>
#include "gameObject.h"

void renderSelectionBar(GLFWwindow* window, int fbWidth, int fbHeight);
void renderGameArea(int fbWidth, int fbHeight, float deltaTime);
void drawCircle(float x, float y, float radius, int segments);
void drawSquare(float x, float y, float size);
void renderTextBox(GameObject* gameObject, int fbWidth, int fbHeight);

#endif // DRAWSHAPE_H
