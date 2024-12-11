// square.h

#ifndef SQUARE_H
#define SQUARE_H

#include "structs.h"

void updateSquare(Square* square, float deltaTime);
void resolve_square_collision(GameObject* obj1, GameObject* obj2);

#endif // SQUARE_H
