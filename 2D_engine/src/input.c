// input.c

#include <math.h>
#include "headers/input.h"
#include "headers/gameObject.h"
#include "headers/drawShape.h"
#include "headers/structs.h"
#include "headers/constants.h"
#include <stdio.h>

ShapeType selectedShapeType = SHAPE_CIRCLE;
int paused = 1;
extern bool dragging;
GameObject* draggedObject = NULL;

void handleSelectionBarClick(double xpos, double ypos, int width, int height);
void handleGameAreaClick(GLFWwindow* window, double xpos, double ypos, int width, int height);
void handleMouseRelease(GLFWwindow* window, double xpos, double ypos, int width, int height);
void handlePauseButtonClick(double xpos, double ypos, int width, int height);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    int width, height;

    glfwGetCursorPos(window, &xpos, &ypos);
    glfwGetWindowSize(window, &width, &height);

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            if (ypos <= height * 0.1 && xpos <= width * 0.2) {
                handlePauseButtonClick(xpos, ypos, width, height);
            }
            else if(ypos <= height * 0.1 && xpos >= width * 0.8){
                clearAllGameObjects();
            }
            else if (xpos <= width * 0.2) {
                handleSelectionBarClick(xpos, ypos, width, height);
            } else {
                draggedObject = getObjectAtCursor(xpos, ypos, width, height);
                if (draggedObject) {
                    dragging = true;
                } else {
                    // Only create a new object if no existing object is clicked
                    handleGameAreaClick(window, xpos, ypos, width, height);
                }
            }
        } else if (action == GLFW_RELEASE) {
            if (dragging && draggedObject) {
                draggedObject = NULL;
            } else {
                handleMouseRelease(window, xpos, ypos, width, height);
            }
            dragging = false;
        }
    }
}

void handleSelectionBarClick(double xpos, double ypos, int width, int height) {
    float sbWidth = width * 0.2f;
    float padding = 20.0f;
    float buttonHeight = 100.0f;
    float yPos = height - buttonHeight - padding;

    // Adjust yPos because window coordinates start from top-left
    ypos = height - ypos;

    if (ypos >= yPos && ypos <= yPos + buttonHeight) {
        selectedShapeType = SHAPE_CIRCLE;
    }

    yPos -= (buttonHeight + padding);
    if (ypos >= yPos && ypos <= yPos + buttonHeight) {
        selectedShapeType = SHAPE_SQUARE;
    }
}

void handlePauseButtonClick(double xpos, double ypos, int width, int height) {
    int topBarHeight = 100; 
    int buttonSize = 60; 
    float padding = 20.0f;

    ypos = height - ypos;

    paused = !paused; 

}

void handleGameAreaClick(GLFWwindow* window, double xpos, double ypos, int width, int height) {
    float gaWidth = width * 0.8f;
    float gaHeight = height;
    float adjustedX = xpos - width * 0.2f;

    float gl_x = (adjustedX / gaWidth) * 2.0f - 1.0f;
    float aspectRatio = gaHeight / gaWidth;
    float gl_y = (1.0f - (ypos / gaHeight) * 2.0f) * aspectRatio;

    dragging = true;

    previewObject.type = selectedShapeType;
    previewObject = (GameObject){
        .type = selectedShapeType,
        .x = gl_x,
        .y = gl_y,
        .vx = 0.0f,
        .vy = 0.0f,
        .r = 1.0f,
        .g = 0.0f,
        .b = 0.0f
    };

    if (selectedShapeType == SHAPE_CIRCLE) {
        previewObject.shape.circle = (Circle){
            .radius = 0.05f,
        };
    } else if (selectedShapeType == SHAPE_SQUARE) {
        previewObject.shape.square = (Square){
            .size = 0.1f,
        };
    }

    dragStartX = gl_x;
    dragStartY = gl_y;
    dragStartTime = glfwGetTime();
}

void handleMouseRelease(GLFWwindow* window, double xpos, double ypos, int width, int height) {
    if (dragging) {
        dragging = false;

        double dragEndTime = glfwGetTime();
        double deltaTime = dragEndTime - dragStartTime;

        if (deltaTime > 0.0) {
            float gaWidth = width * 0.8f;
            float gaHeight = height;
            float adjustedX = xpos - width * 0.2f;

            float gl_x = (adjustedX / gaWidth) * 2.0f - 1.0f;
            float aspectRatio = gaHeight / gaWidth;
            float gl_y = (1.0f - (ypos / gaHeight) * 2.0f) * aspectRatio;

            float deltaX = gl_x - dragStartX;
            float deltaY = gl_y - dragStartY;

            float velocityScale = 2.0f;
            previewObject.vx = deltaX / deltaTime * velocityScale;
            previewObject.vy = deltaY / deltaTime * velocityScale;

            addGameObject(previewObject);
        }
    }
}

GameObject* getObjectAtCursor(double xpos, double ypos, int width, int height) {
    float gaWidth = width * 0.8f;
    float gaHeight = height;
    float adjustedX = xpos - width * 0.2f;

    float gl_x = (adjustedX / gaWidth) * 2.0f - 1.0f;
    float aspectRatio = gaHeight / gaWidth;
    float gl_y = (1.0f - (ypos / gaHeight) * 2.0f) * aspectRatio;

    for (int i = 0; i < gameObjectCount; i++) {
        GameObject* obj = &gameObjects[i];

        if (obj->type == SHAPE_CIRCLE) {
            float dx = gl_x - obj->x;
            float dy = gl_y - obj->y;
            float distance = sqrtf(dx * dx + dy * dy);
            if (distance <= obj->shape.circle.radius) {
                return obj;
            }
        } else if (obj->type == SHAPE_SQUARE) {
            float halfSize = obj->shape.square.size / 2.0f;
            if (gl_x >= obj->x - halfSize && gl_x <= obj->x + halfSize &&
                gl_y >= obj->y - halfSize && gl_y <= obj->y + halfSize) {
                return obj;
            }
        }
    }

    return NULL; // No object found under cursor
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (dragging && draggedObject) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float gaWidth = width * 0.8f;
        float gaHeight = height;
        float adjustedX = xpos - width * 0.2f;

        float gl_x = (adjustedX / gaWidth) * 2.0f - 1.0f;
        float aspectRatio = gaHeight / gaWidth;
        float gl_y = (1.0f - (ypos / gaHeight) * 2.0f) * aspectRatio;

        // Update the position of the dragged object
        draggedObject->x = gl_x;
        draggedObject->y = gl_y;
    }
}

