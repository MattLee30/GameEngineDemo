// square.c

#include "headders/square.h"
#include "headders/constants.h"
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

void handleSquareCollisions(Square* squareA, Square* squareB) {
    // Implement axis-aligned bounding box (AABB) collision detection
    float dx = fabsf(squareA->x - squareB->x);
    float dy = fabsf(squareA->y - squareB->y);
    float overlapX = (squareA->size / 2 + squareB->size / 2) - dx;
    float overlapY = (squareA->size / 2 + squareB->size / 2) - dy;

    if (overlapX > 0 && overlapY > 0) {
        // Collision detected
        // Resolve collision by moving squares apart
        if (overlapX < overlapY) {
            // Resolve along x-axis
            float sign = (squareA->x < squareB->x) ? -1.0f : 1.0f;
            squareA->x += sign * overlapX / 2.0f;
            squareB->x -= sign * overlapX / 2.0f;

            // Exchange velocities along x-axis
            float tempVx = squareA->vx;
            squareA->vx = squareB->vx * BOUNCE_DAMPING;
            squareB->vx = tempVx * BOUNCE_DAMPING;
        } else {
            // Resolve along y-axis
            float sign = (squareA->y < squareB->y) ? -1.0f : 1.0f;
            squareA->y += sign * overlapY / 2.0f;
            squareB->y -= sign * overlapY / 2.0f;

            // Exchange velocities along y-axis
            float tempVy = squareA->vy;
            squareA->vy = squareB->vy * BOUNCE_DAMPING;
            squareB->vy = tempVy * BOUNCE_DAMPING;
        }
    }
}
