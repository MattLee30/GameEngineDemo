// square.h

#ifndef SQUARE_H
#define SQUARE_H

#include "structs.h"

void updateSquare(Square* square, float deltaTime);
void handleSquareCollisions(Square* squareA, Square* squareB);
void handleCircleSquareCollision(Circle* circle, Square* square);

#endif // SQUARE_H
