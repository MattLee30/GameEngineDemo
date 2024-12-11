// square.c

#include "headers/square.h"
#include "headers/constants.h"
#include <math.h>
#include <stdio.h>


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

void handleCircleSquareCollision(Circle* circle, Square* square) {
    float circleX = circle->gameObject->x;
    float circleY = circle->gameObject->y;
    float circleRadius = circle->radius;

    float squareX = square->gameObject->x;
    float squareY = square->gameObject->y;
    float squareSize = square->size;

    // Find the closest point on the square to the circle center
    float closestX = fmaxf(squareX - squareSize / 2, fminf(circleX, squareX + squareSize / 2));
    float closestY = fmaxf(squareY - squareSize / 2, fminf(circleY, squareY + squareSize / 2));

    // Calculate the distance between the circle's center and the closest point on the square
    float dx = circleX - closestX;
    float dy = circleY - closestY;
    float distance = sqrtf(dx * dx + dy * dy);

    // If the distance between the circle's center and the closest point is less than the radius, collision detected
    if (distance < circleRadius) {
        float overlap = circleRadius - distance;

        // Normalize the direction of the collision (from the circle center to the closest point)
        float nx = dx / distance;
        float ny = dy / distance;

        // Resolve the overlap by moving the circle away from the square
        circle->gameObject->x += overlap * nx;
        circle->gameObject->y += overlap * ny;

        // Now handle the velocity exchange (assuming elastic collision)
        float rvx = square->gameObject->vx - circle->gameObject->vx;
        float rvy = square->gameObject->vy - circle->gameObject->vy;

        // Relative velocity along the normal
        float vn = rvx * nx + rvy * ny;

        // If velocities are separating, do nothing
        if (vn > 0.0f)
            return;

        // Coefficient of restitution (elasticity)
        float restitution = BOUNCE_DAMPING;

        // Impulse scalar
        float impulse = -(1.0f + restitution) * vn;
        impulse /= 2.0f; // Assuming mass = 1 for both

        // Apply impulse to both objects' velocities
        float impulseX = impulse * nx;
        float impulseY = impulse * ny;

        circle->gameObject->vx -= impulseX;
        circle->gameObject->vy -= impulseY;
        square->gameObject->vx += impulseX;
        square->gameObject->vy += impulseY;
    }
}

