#include <stdlib.h>
#include <math.h>
#include "headers/gameObject.h"
#include "headers/circle.h"
#include "headers/square.h"
#include "headers/drawShape.h"
#include "headers/constants.h"

// Global variables
GameObject* gameObjects = NULL;
int gameObjectCount = 0;

int selectedObjectIndex = -1;
GameObject previewObject;

bool dragging = false;
bool isDraggingObject = false;
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
        drawSquare(square->x, square->y, square->size);
    }
}

// Function to update GameObject positions
void updateGameObject(GameObject* obj, float deltaTime) {
    if (obj->type == SHAPE_CIRCLE) {
        updateBall(&obj->shape.circle, deltaTime, GRAVITY);
    } else if (obj->type == SHAPE_SQUARE) {
        updateSquare(&obj->shape.square, deltaTime);
    }
}

// Function to handle collisions between GameObjects
void handleGameObjectCollisions() {
    for (int i = 0; i < gameObjectCount; i++) {
        if (isDraggingObject && i == selectedObjectIndex) continue;

        GameObject* objA = &gameObjects[i];

        for (int j = i + 1; j < gameObjectCount; j++) {
            if (isDraggingObject && j == selectedObjectIndex) continue;

            GameObject* objB = &gameObjects[j];

            // Handle different collision cases
            // if (objA->type == SHAPE_CIRCLE && objB->type == SHAPE_CIRCLE) {
            //     handleCollisions(&objA->shape.circle, &objB->shape.circle);
            // } 
            // else if (objA->type == SHAPE_SQUARE && objB->type == SHAPE_SQUARE) {
            //     handleSquareCollisions(&objA->shape.square, &objB->shape.square);
            // }
            // You can add circle-square collision handling here
        }
    }
}

// Function to initialize balls
void initializeBalls(int numBalls) {
    for (int i = 0; i < numBalls; i++) {
        GameObject newObject;
        newObject.type = SHAPE_CIRCLE;
        newObject.shape.circle = (Circle){
            .x = -0.5f + i * 0.1f,
            .y = 0.5f,
            .radius = 0.05f,
            .vx = 0.1f * (i % 2 == 0 ? 1 : -1),
            .vy = 0.0f,
            .r = 1.0f,
            .g = 0.0f,
            .b = 0.0f
        };
        addGameObject(newObject);
    }
}
