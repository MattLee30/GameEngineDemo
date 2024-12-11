// input.h

#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include "structs.h"

extern ShapeType selectedShapeType;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
GameObject* getObjectAtCursor(double xpos, double ypos, int width, int height);

#endif // INPUT_H
