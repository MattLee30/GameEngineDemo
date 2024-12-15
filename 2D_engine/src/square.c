// square.c

#include "headers/square.h"
#include "headers/constants.h"
#include <math.h>
#include <stdio.h>
#include "headers/Vector2.h"


void updateSquare(Square* square, float deltaTime) {
    // Update position
    square->gameObject->x += square->gameObject->vx * deltaTime;
    square->gameObject->y += square->gameObject->vy * deltaTime;

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