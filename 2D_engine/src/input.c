// input.c

#include <math.h>
#include "headers/input.h"
#include "headers/gameObject.h"
#include "headers/drawShape.h"
#include "headers/structs.h"
#include "headers/constants.h"
#include <stdio.h>

ShapeType selectedShapeType = SHAPE_CIRCLE; // Default shape
int paused = 1; // Pause state
extern bool dragging;

// Function prototypes
void handleSelectionBarClick(double xpos, double ypos, int width, int height);
void handleGameAreaClick(GLFWwindow* window, double xpos, double ypos, int width, int height);
void handleMouseRelease(GLFWwindow* window, double xpos, double ypos, int width, int height);
void handlePauseButtonClick(double xpos, double ypos, int width, int height);

// Mouse button callback for object creation and selection
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    int width, height;

    glfwGetCursorPos(window, &xpos, &ypos);
    glfwGetWindowSize(window, &width, &height);

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS && !dragging) {
            if (ypos <= height * 0.1) {
                // Click in the top bar (Pause button)
                handlePauseButtonClick(xpos, ypos, width, height);
            } else if (xpos <= width * 0.2) {
                // Click in the selection bar
                handleSelectionBarClick(xpos, ypos, width, height);
            } else {
                // Click in the game area
                handleGameAreaClick(window, xpos, ypos, width, height);
            }
        } else if (action == GLFW_RELEASE && dragging) {
            // Handle mouse release
            handleMouseRelease(window, xpos, ypos, width, height);
        }
    }
}


void handleSelectionBarClick(double xpos, double ypos, int width, int height) {
    // Calculate button positions
    float sbWidth = width * 0.2f;
    float padding = 20.0f;
    float buttonHeight = 100.0f;
    float yPos = height - buttonHeight - padding;

    // Adjust yPos because window coordinates start from top-left
    ypos = height - ypos;

    // Check if Circle Button was clicked
    if (ypos >= yPos && ypos <= yPos + buttonHeight) {
        selectedShapeType = SHAPE_CIRCLE;
    }

    // Check if Square Button was clicked
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

    // if (ypos >= height - topBarHeight && ypos <= height - topBarHeight + buttonSize) {
    //     printf("HELP1\n");

    //     if (xpos >= width - buttonSize - padding && xpos <= width - padding) {
    //         printf("HELP2\n");

    //         paused = !paused; 
    //     }
    // }

    paused = !paused; 

}

void handleGameAreaClick(GLFWwindow* window, double xpos, double ypos, int width, int height) {
    // Adjust for game area viewport
    float gaWidth = width * 0.8f;
    float gaHeight = height;
    float adjustedX = xpos - width * 0.2f;

    float gl_x = (adjustedX / gaWidth) * 2.0f - 1.0f;
    float aspectRatio = gaHeight / gaWidth;
    float gl_y = (1.0f - (ypos / gaHeight) * 2.0f) * aspectRatio;

    // Start dragging to create a new object
    dragging = true;

    // Initialize the preview object
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
        // Finish creating the new object
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

            if (previewObject.type == SHAPE_CIRCLE) {
                previewObject.vx = deltaX / deltaTime * velocityScale;
                previewObject.vy = deltaY / deltaTime * velocityScale;
            } else if (previewObject.type == SHAPE_SQUARE) {
                previewObject.vx = deltaX / deltaTime * velocityScale;
                previewObject.vy = deltaY / deltaTime * velocityScale;
            }

            // Add the preview object to the game
            addGameObject(previewObject);
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // Implement cursor tracking if needed
    int width, height;

    if(ypos - previewObject.y <= 0 && xpos - previewObject.x <= 0){
        printf("HELP\n");
    }

    glfwGetCursorPos(window, &xpos, &ypos);
    glfwGetWindowSize(window, &width, &height);

    //handle hover events
    if (ypos <= height * 0.1) {
        // Click in the top bar (Pause button)
        // handlePauseButtonClick(xpos, ypos, width, height);
    } else if (xpos <= width * 0.2) {
        // Click in the selection bar
        // handleSelectionBarClick(xpos, ypos, width, height);
    } 
    // else {
    //     // Click in the game area
    //     handleGameAreaClick(window, xpos, ypos, width, height);
    // }
}
