#ifndef SQUARE_H
#define SQUARE_H

#include <GLFW/glfw3.h>
#include "structs.h"

extern Square* squares;
extern int squareCount;

void updateSquare(Square* square, float deltaTime);

void handleCollisions();

// Function to draw a square
void drawSquare(float x, float y, float size) {
    float halfSize = size / 2.0f;  // Half of the square's size

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); 
    glVertex2f(x - halfSize, y - halfSize);  
    glVertex2f(x + halfSize, y - halfSize);  
    glVertex2f(x + halfSize, y + halfSize);  
    glVertex2f(x - halfSize, y + halfSize);  
    glVertex2f(x - halfSize, y - halfSize); 
    glEnd();
}

#endif // SQUARE_H
