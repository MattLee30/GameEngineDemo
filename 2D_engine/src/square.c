// square.c

#include "headers/square.h"
#include "headers/constants.h"
#include <math.h>


void updateSquare(Square* square, float deltaTime) {
    // Update position
    square->x += square->vx * deltaTime;
    square->y += square->vy * deltaTime;

    // Apply gravity
    square->vy += GRAVITY * deltaTime;

    // Handle window boundaries
    // Left and right
    if (square->x - square->size / 2 < -1.0f) {
        square->x = -1.0f + square->size / 2;
        square->vx = -square->vx * BOUNCE_DAMPING;
    } else if (square->x + square->size / 2 > 1.0f) {
        square->x = 1.0f - square->size / 2;
        square->vx = -square->vx * BOUNCE_DAMPING;
    }

    // Bottom and top
    if (square->y - square->size / 2 < -1.0f) {
        square->y = -1.0f + square->size / 2;
        square->vy = -square->vy * BOUNCE_DAMPING;
    } else if (square->y + square->size / 2 > 1.0f) {
        square->y = 1.0f - square->size / 2;
        square->vy = -square->vy * BOUNCE_DAMPING;
    }
}