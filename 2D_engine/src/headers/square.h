// square.h

#ifndef SQUARE_H
#define SQUARE_H

#include "structs.h"

void updateSquare(Square* square, float deltaTime);
void handleSquareCollisions(Square* squareA, Square* squareB);

#endif // SQUARE_H
