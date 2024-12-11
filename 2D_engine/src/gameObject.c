#include <stdlib.h>
#include <math.h>
#include "headers/gameObject.h"
#include "headers/circle.h"
#include "headers/square.h"
#include "headers/drawShape.h"
#include "headers/constants.h"
#include <stdio.h>

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
    if (!gameObjects) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // For memory allocation safety, reassign the gameObject pointers
    for (int i = 0; i < gameObjectCount; i++) {
        if (gameObjects[i].type == SHAPE_CIRCLE) {
            gameObjects[i].shape.circle.gameObject = &gameObjects[i];
        } else if (gameObjects[i].type == SHAPE_SQUARE) {
            gameObjects[i].shape.square.gameObject = &gameObjects[i];
        }
    }

    gameObjects[gameObjectCount] = newObject;

    if (newObject.type == SHAPE_CIRCLE) {
        gameObjects[gameObjectCount].shape.circle.gameObject = &gameObjects[gameObjectCount];
    } else if (newObject.type == SHAPE_SQUARE) {
        gameObjects[gameObjectCount].shape.square.gameObject = &gameObjects[gameObjectCount];
    }
    gameObjectCount++;
}

// Function to draw a GameObject
void drawGameObject(const GameObject* obj) {
    glColor3f(obj->r, obj->g, obj->b);
    if (obj->type == SHAPE_CIRCLE) {
        const Circle* circle = &obj->shape.circle;
        drawCircle(obj->x, obj->y, circle->radius, 100);
    } 
    else if (obj->type == SHAPE_SQUARE) {
        const Square* square = &obj->shape.square;
        drawSquare(obj->x, obj->y, square->size);
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
            if (objA->type == SHAPE_CIRCLE && objB->type == SHAPE_CIRCLE) {
                handleCollisions(&objA->shape.circle, &objB->shape.circle);
            } else if (objA->type == SHAPE_SQUARE && objB->type == SHAPE_SQUARE) {
                handleSquareCollisions(&objA->shape.square, &objB->shape.square);
            }
        }
    }
}

// Function to initialize balls
// void initializeBalls(int numBalls) {
//     for (int i = 0; i < numBalls; i++) {
//         GameObject newObject;
//         newObject = (GameObject){
//             .type = SHAPE_CIRCLE,
//             .x = -0.5f + i * 0.1f,
//             .y = 0.5f,
//             .vx = 0.1f * (i % 2 == 0 ? 1 : -1),
//             .vy = 0.0f,
//             .r = 1.0f,
//             .g = 0.0f,
//             .b = 0.0f
//         };
//         newObject.shape.circle = (Circle){
//             .radius = 0.05f,
//             .gameObject = NULL
//         };
//         addGameObject(newObject);

//         gameObjects[gameObjectCount - 1].shape.circle.gameObject = &gameObjects[gameObjectCount - 1];
//     }
// }
