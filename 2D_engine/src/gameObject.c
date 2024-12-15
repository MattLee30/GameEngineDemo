#include <stdlib.h>
#include <math.h>
#include "headers/gameObject.h"
#include "headers/circle.h"
#include "headers/square.h"
#include "headers/drawShape.h"
#include "headers/constants.h"
#include <stdio.h>

GameObject* gameObjects = NULL;
int gameObjectCount = 0;

int selectedObjectIndex = -1;
GameObject previewObject;

bool dragging = false;
bool isDraggingObject = false;
double dragStartX, dragStartY;
double dragStartTime;

void resolve_collision(GameObject* obj1, GameObject* obj2);
bool circle_square_collide(Circle* circle, Square* square);
bool circle_circle_collide(Circle* circle1, Circle* circle2);
bool square_square_collide(Square* square1, Square* square2);

void addGameObject(GameObject newObject) {
    gameObjects = (GameObject*)realloc(gameObjects, (gameObjectCount + 1) * sizeof(GameObject));
    if (!gameObjects) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // For memory allocation safety, reassign the gameObject pointers -- DO NOT REMOVE
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
    printf("Added new object. Total objects: %d\n", gameObjectCount);
}

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

void updateGameObject(GameObject* obj, float deltaTime) {
    if (obj->type == SHAPE_CIRCLE) {
        updateBall(&obj->shape.circle, deltaTime);
    } else if (obj->type == SHAPE_SQUARE) {
        updateSquare(&obj->shape.square, deltaTime);
    }
}

void handleGameObjectCollisions() {
    for (int i = 0; i < gameObjectCount; i++) {
        if (isDraggingObject && i == selectedObjectIndex) continue;

        if (selectedObjectIndex < 0 || selectedObjectIndex >= gameObjectCount) {
            selectedObjectIndex = -1; 
        }

        GameObject* objA = &gameObjects[i];

        for (int j = i + 1; j < gameObjectCount; j++) {
            if (isDraggingObject && j == selectedObjectIndex) continue;

            GameObject* objB = &gameObjects[j];

            if(circle_circle_collide(&objA->shape.circle, &objB->shape.circle) ||
               square_square_collide(&objA->shape.square, &objB->shape.square) || 
               circle_square_collide(&objA->shape.circle, &objB->shape.square)) {
                    resolve_collision(objA, objB);
            }
        }
    }
}

void clearAllGameObjects() {
    free(gameObjects); 
    gameObjects = NULL; 
    gameObjectCount = 0; 
    printf("All game objects cleared.\n");
}