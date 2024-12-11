// square.c

#include "headers/square.h"
#include "headers/constants.h"
#include <math.h>


void updateSquare(Square* square, float deltaTime) {
    // Update position
    square->gameObject->x += square->gameObject->vx * deltaTime;
    square->gameObject->y += square->gameObject->vy * deltaTime;

    // Apply gravity
    square->gameObject->vy += GRAVITY * deltaTime;

    // Handle window boundaries
    // Left and right
    if (square->gameObject->x - square->size / 2 < -1.0f) {
        square->gameObject->x = -1.0f + square->size / 2;
        square->gameObject->vx = -square->gameObject->vx * BOUNCE_DAMPING;
    } else if (square->gameObject->x + square->size / 2 > 1.0f) {
        square->gameObject->x = 1.0f - square->size / 2;
        square->gameObject->vx = -square->gameObject->vx * BOUNCE_DAMPING;
    }

    // Bottom and top
    if (square->gameObject->y - square->size / 2 < -1.0f) {
        square->gameObject->y = -1.0f + square->size / 2;
        square->gameObject->vy = -square->gameObject->vy * BOUNCE_DAMPING;
    } else if (square->gameObject->y + square->size / 2 > 1.0f) {
        square->gameObject->y = 1.0f - square->size / 2;
        square->gameObject->vy = -square->gameObject->vy * BOUNCE_DAMPING;
    }
}

void handleSquareCollisions(Square* squareA, Square* squareB) {
    // Implement axis-aligned bounding box (AABB) collision detection
    float dx = fabsf(squareA->gameObject->x - squareB->gameObject->x);
    float dy = fabsf(squareA->gameObject->y - squareB->gameObject->y);
    float overlapX = (squareA->size / 2 + squareB->size / 2) - dx;
    float overlapY = (squareA->size / 2 + squareB->size / 2) - dy;

    if (overlapX > 0 && overlapY > 0) {
        // Collision detected
        // Resolve collision by moving squares apart
        if (overlapX < overlapY) {
            // Resolve along x-axis
            float sign = (squareA->gameObject->x < squareB->gameObject->x) ? -1.0f : 1.0f;
            squareA->gameObject->x += sign * overlapX / 2.0f;
            squareB->gameObject->x -= sign * overlapX / 2.0f;

            // Exchange velocities along x-axis
            float tempVx = squareA->gameObject->vx;
            squareA->gameObject->vx = squareB->gameObject->vx * BOUNCE_DAMPING;
            squareB->gameObject->vx = tempVx * BOUNCE_DAMPING;
        } else {
            // Resolve along y-axis
            float sign = (squareA->gameObject->y < squareB->gameObject->y) ? -1.0f : 1.0f;
            squareA->gameObject->y += sign * overlapY / 2.0f;
            squareB->gameObject->y -= sign * overlapY / 2.0f;

            // Exchange velocities along y-axis
            float tempVy = squareA->gameObject->vy;
            squareA->gameObject->vy = squareB->gameObject->vy * BOUNCE_DAMPING;
            squareB->gameObject->vy = tempVy * BOUNCE_DAMPING;
        }
    }
}
