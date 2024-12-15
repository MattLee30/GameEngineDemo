// gameObject.h

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdbool.h>
#include "structs.h"
#include "constants.h"

extern GameObject* gameObjects;
extern int gameObjectCount;
extern bool dragging;
extern bool isDraggingObject;
extern int selectedObjectIndex;
extern GameObject previewObject;
extern double dragStartX, dragStartY;
extern double dragStartTime;

void addGameObject(GameObject newObject);
void drawGameObject(const GameObject* obj);
void updateGameObject(GameObject* obj, float deltaTime);
void handleGameObjectCollisions();
void initializeBalls(int numBalls);

#endif // GAMEOBJECT_H
