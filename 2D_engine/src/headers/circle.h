// circle.h

#ifndef CIRCLE_H
#define CIRCLE_H

#include "structs.h"

void updateBall(Circle* circle, float deltaTime);
void handleCollisions(Circle* circleA, Circle* circleB);

#endif // CIRCLE_H
