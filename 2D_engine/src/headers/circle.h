// circle.h

#ifndef CIRCLE_H
#define CIRCLE_H

#include "structs.h"

void updateBall(Circle* circle, float deltaTime);
void resolve_circle_collision(GameObject* obj1, GameObject* obj2);

#endif // CIRCLE_H
