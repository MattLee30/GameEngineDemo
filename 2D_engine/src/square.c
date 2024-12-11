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

// void resolve_square_collision(GameObject* obj1, GameObject* obj2) {
//     Vector2 normal = vector_subtract((Vector2){obj2->x, obj2->y}, (Vector2){obj1->x, obj1->y});
//     normal = vector_normalize(normal);

//     Vector2 relative_velocity = vector_subtract((Vector2){obj1->vx, obj1->vy}, 
//                                                 (Vector2){obj2->vx, obj2->vy});

//     float relative_speed = vector_dot(relative_velocity, normal);

//     if (relative_speed > 0) {
//         return;
//     }

//     float impulse = -(1 + 1) * relative_speed / (1 + 1);  // Restitution factor e = 1

//     Vector2 impulse_vector = vector_scale(normal, impulse);

//     obj1->vx += impulse_vector.x;
//     obj1->vy += impulse_vector.y;

//     obj2->vx -= impulse_vector.x;
//     obj2->vy -= impulse_vector.y;
// }
