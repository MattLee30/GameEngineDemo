#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headders/window.h"
#include "headders/framebuffer.h"
#include "headders/structs.h"
#include "headders/circle.h"
#include "headders/square.h"
#include "headders/drawShape.h"


#define GRAVITY -5.8f  
#define BOUNCE_DAMPING 0.8f 

GameObject* gameObjects = NULL;  // Array of GameObjects
int gameObjectCount = 0;

Circle* balls = NULL;
int ballCount = 0;

bool dragging = false;           
bool isDraggingObject = false;   
int selectedObjectIndex = -1;    

GameObject previewObject;        // Preview object for creation
double dragStartX, dragStartY;
double dragStartTime;

// Function to add a new GameObject
void addGameObject(GameObject newObject) {
    gameObjects = (GameObject*)realloc(gameObjects, (gameObjectCount + 1) * sizeof(GameObject));
    gameObjects[gameObjectCount] = newObject;
    gameObjectCount++;
}

// Function to draw a GameObject
void drawGameObject(const GameObject* obj) {
    if (obj->type == SHAPE_CIRCLE) {
        Circle* circle = &obj->shape.circle;
        glColor3f(circle->r, circle->g, circle->b);  // Set color before drawing
        drawCircle(circle->x, circle->y, circle->radius, 100);
    } else if (obj->type == SHAPE_SQUARE) {
        Square* square = &obj->shape.square;
        glColor3f(square->r, square->g, square->b);  // Set color before drawing
        // drawSquare(square->x, square->y, square->size);
    }
}


// Function to update GameObject positions
void updateGameObject(GameObject* obj, float deltaTime) {
    if (obj->type == SHAPE_CIRCLE) {
        updateBall(&obj->shape.circle, deltaTime, GRAVITY);
    } else if (obj->type == SHAPE_SQUARE) {
        //TODO
        // updateSquare(&obj->shape.square, deltaTime);
    }
}

// Function to handle collisions between GameObjects
void handleGameObjectCollisions() {
    for (int i = 0; i < gameObjectCount; i++) {
        if (isDraggingObject && i == selectedObjectIndex) continue;

        GameObject* objA = &gameObjects[i];
        if (objA->type != SHAPE_CIRCLE) continue;

        for (int j = i + 1; j < gameObjectCount; j++) {
            if (isDraggingObject && j == selectedObjectIndex) continue;

            GameObject* objB = &gameObjects[j];
            if (objB->type != SHAPE_CIRCLE) continue;

            // Handle collisions between two circles
            //TODO
            handleCollisions(&objA->shape.circle, &objB->shape.circle);
        }
    }
}

// Mouse button callback for object creation and dragging
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        double xpos, ypos;
        int width, height;

        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &width, &height);

        float gl_x = (float)((xpos / width) * 2.0f - 1.0f);
        float gl_y = (float)(1.0f - (ypos / height) * 2.0f);

        if (action == GLFW_PRESS) {
            // Check for existing object selection
            for (int i = 0; i < gameObjectCount; i++) {
                if (gameObjects[i].type == SHAPE_CIRCLE) {
                    Circle* circle = &gameObjects[i].shape.circle;
                    float dx = gl_x - circle->x;
                    float dy = gl_y - circle->y;
                    float distance = sqrtf(dx * dx + dy * dy);

                    if (distance <= circle->radius) {
                        selectedObjectIndex = i;
                        isDraggingObject = true;
                        dragging = false;
                        break;
                    }
                }
            }

            if (!isDraggingObject) {
                // Start dragging to create a new object
                dragging = true;

                // Initialize a circle object as the preview
                previewObject.type = SHAPE_CIRCLE;
                previewObject.shape.circle = (Circle){gl_x, gl_y, 0.05f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

                dragStartX = gl_x;
                dragStartY = gl_y;
                dragStartTime = glfwGetTime();
            }
        } else if (action == GLFW_RELEASE) {
            if (isDraggingObject) {
                // Stop dragging the selected object
                isDraggingObject = false;
                selectedObjectIndex = -1;
            } else if (dragging) {
                // Finalize the new object
                dragging = false;

                double dragEndTime = glfwGetTime();
                double deltaTime = dragEndTime - dragStartTime;

                if (deltaTime > 0.0) {
                    float velocityScale = 2.0f;
                    float deltaX = gl_x - dragStartX;
                    float deltaY = gl_y - dragStartY;

                    previewObject.shape.circle.vx = deltaX / deltaTime * velocityScale;
                    previewObject.shape.circle.vy = deltaY / deltaTime * velocityScale;

                    // Add the preview object to the game
                    addGameObject(previewObject);
                }
            }
        }
    }
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float gl_x = (float)((xpos / width) * 2.0f - 1.0f);
    float gl_y = (float)(1.0f - (ypos / height) * 2.0f);

    if (isDraggingObject && selectedObjectIndex >= 0 && selectedObjectIndex < gameObjectCount) {
        if (gameObjects[selectedObjectIndex].type == SHAPE_CIRCLE) {
            Circle* circle = &gameObjects[selectedObjectIndex].shape.circle;
            circle->x = gl_x;
            circle->y = gl_y;
            circle->vx = 0.0f;
            circle->vy = 0.0f;
        }
    } else if (dragging) {
        previewObject.shape.circle.x = gl_x;
        previewObject.shape.circle.y = gl_y;
    }
}




// Main function
int main() {
    if (!glfwInit()) {
        return -1;
    }


    GLFWwindow* window = initializeWindow("GameObject Demo", 800, 600,
                                          framebuffer_size_callback,
                                          mouse_button_callback,
                                          cursor_position_callback);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    GLFWwindow* window2 = initializeWindow("Drawing Window", 800, 600,
                                           framebuffer_size_callback, // You can use a different callback if needed
                                           NULL, NULL); // No mouse or cursor callbacks for window 2
    if (!window2) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
            // Setup window 1 context
        glfwMakeContextCurrent(window);
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        framebuffer_size_callback(window, fbWidth, fbHeight);

        // Setup window 2 context
        glfwMakeContextCurrent(window2);
        initializeDrawing(window2);  // Your drawing module's initialization

        // Time variables
        float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window) && !glfwWindowShouldClose(window2)) {
        glfwPollEvents();

        // Calculate time delta
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        /*** Rendering for Window 1 ***/
        glfwMakeContextCurrent(window);

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Ensure the modelview matrix is reset
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Update game objects
        for (int i = 0; i < gameObjectCount; i++) {
            if (isDraggingObject && i == selectedObjectIndex) continue;

            updateGameObject(&gameObjects[i], deltaTime);
        }

        // Handle collisions
        handleGameObjectCollisions();

        // Draw game objects
        for (int i = 0; i < gameObjectCount; i++) {
            drawGameObject(&gameObjects[i]);
        }

        if (dragging) {
            drawGameObject(&previewObject);
        }

        glfwSwapBuffers(window);

        /*** Rendering for Window 2 ***/
        glfwMakeContextCurrent(window2);

        glClear(GL_COLOR_BUFFER_BIT);

        // Render the drawing in window2
        renderDrawing();

        glfwSwapBuffers(window2);
    }
    free(gameObjects);
    gameObjects = NULL;

    glfwDestroyWindow(window);
    glfwDestroyWindow(window2);
    glfwTerminate();
    return 0;
}

