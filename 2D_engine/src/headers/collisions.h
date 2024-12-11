#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "gameObject.h"
#include "circle.h"
#include "square.h"

bool circle_square_collide(Circle* circle, Square* square);
void resolve_collision(GameObject* obj1, GameObject* obj2);
bool circle_circle_collide(Circle* circle1, Circle* circle2);

#endif //COLLISIONS_H
