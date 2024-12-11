// input.c

#include <math.h>
#include "headers/input.h"
#include "headers/gameobject.h"
#include "headers/drawShape.h"
#include "headers/structs.h"
#include "headers/constants.h"
#include <stdio.h>
#include "headers/textrenderer.h"

ShapeType selectedShapeType = SHAPE_CIRCLE; // Default shape
int paused = 1; // Pause state
extern bool dragging;
GameObject* draggedObject = NULL;
GameObject* selectedObject = NULL;

// Function prototypes
void handleSelectionBarClick(double xpos, double ypos, int width, int height);
void handleGameAreaClick(GLFWwindow* window, double xpos, double ypos, int width, int height);
void handleMouseRelease(GLFWwindow* window, double xpos, double ypos, int width, int height);
void handlePauseButtonClick(double xpos, double ypos, int width, int height);

// void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//     double xpos, ypos;
//     int width, height;

//     glfwGetCursorPos(window, &xpos, &ypos);
//     glfwGetWindowSize(window, &width, &height);

//     if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//         if (paused) {
//             // Check if a game object is clicked
//             selectedObject = getObjectAtCursor(xpos, ypos, width, height);
//         } else {
//             handleSelectionBarClick(xpos, ypos, width, height);
//             draggedObject = getObjectAtCursor(xpos, ypos, width, height);
//             if (draggedObject) {
//                 dragging = true;
//             } else {
//                 handleGameAreaClick(window, xpos, ypos, width, height);
//             }
//         }
//     } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
//         if (dragging && draggedObject) {
//             draggedObject = NULL;
//         } else {
//             handleMouseRelease(window, xpos, ypos, width, height);
//         }
//         dragging = false;
//     }

// Mouse button callback for object creation and selection
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            // if (ypos <= height * 0.1) {
            //     // Click in the top bar (Pause button)
            //     handlePauseButtonClick(xpos, ypos, width, height);
            // } else 
            // if (xpos <= width * 0.2) {
            //     // Click in the selection bar
            //     handleSelectionBarClick(xpos, ypos, width, height);
            // } else {
            //     // Click in the game area (not in the pause bar or selection bar)
            //     draggedObject = getObjectAtCursor(xpos, ypos, width, height);
            //     if (draggedObject) {
            //         dragging = true; // Start dragging the existing object
            //     } else {
            //         // Create a new object if no existing object is clicked
            //         handleGameAreaClick(window, xpos, ypos, width, height);
            //     }
            // }
            handleGameAreaClick(window, xpos, ypos, width, height);

        } else if (action == GLFW_RELEASE) {
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

    // // Render the text box if the game is paused
    // if (paused && selectedObject) {
    //     renderTextBox(selectedObject, xpos, ypos);
    // }

}

void handlePauseButtonClick(double xpos, double ypos, int width, int height) {
    int topBarHeight = 100; 
    int buttonSize = 60; 
    float padding = 20.0f;

    ypos = height - ypos;

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
    if (selectedShapeType == SHAPE_CIRCLE) {
        previewObject.shape.circle = (Circle){
            .x = gl_x,
            .y = gl_y,
            .radius = 0.05f,
            .vx = 0.0f,
            .vy = 0.0f,
            .r = 1.0f,
            .g = 0.0f,
            .b = 0.0f
        };
    } else if (selectedShapeType == SHAPE_SQUARE) {
        previewObject.shape.square = (Square){
            .x = gl_x,
            .y = gl_y,
            .size = 0.1f,
            .vx = 0.0f,
            .vy = 0.0f,
            .r = 0.0f,
            .g = 0.0f,
            .b = 1.0f
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
                previewObject.shape.circle.vx = deltaX / deltaTime * velocityScale;
                previewObject.shape.circle.vy = deltaY / deltaTime * velocityScale;
            } else if (previewObject.type == SHAPE_SQUARE) {
                previewObject.shape.square.vx = deltaX / deltaTime * velocityScale;
                previewObject.shape.square.vy = deltaY / deltaTime * velocityScale;
            }

            // Add the preview object to the game
            addGameObject(previewObject);
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // Implement cursor tracking if needed
}
