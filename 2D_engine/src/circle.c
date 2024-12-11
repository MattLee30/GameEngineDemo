// circle.c

#include <math.h>
#include "headers/circle.h"
#include "headers/constants.h"
#include "headers/gameObject.h"
#include <stdio.h>
#include "headers/Vector2.h"

extern GameObject* gameObject;

void updateBall(Circle* circle, float deltaTime) {
    if (!circle || !circle->gameObject) {
        fprintf(stderr, "Error: Circle or its GameObject is NULL.\n");
        return;
    }
    // Update position based on velocity and deltaTime
    circle->gameObject->x += circle->gameObject->vx * deltaTime;
    circle->gameObject->y += circle->gameObject->vy * deltaTime;

    // Apply gravity
    circle->gameObject->vy += GRAVITY * deltaTime;

    // Handle collisions with window boundaries
    if (circle->gameObject->x - circle->radius < -1.0f) {
        circle->gameObject->x = -1.0f + circle->radius;
        circle->gameObject->vx = -circle->gameObject->vx * BOUNCE_DAMPING;
    } else if (circle->gameObject->x + circle->radius > 1.0f) {
        circle->gameObject->x = 1.0f - circle->radius;
        circle->gameObject->vx = -circle->gameObject->vx * BOUNCE_DAMPING;
    }

    if (circle->gameObject->y - circle->radius < -1.0f) {
        circle->gameObject->y = -1.0f + circle->radius;
        circle->gameObject->vy = -circle->gameObject->vy * BOUNCE_DAMPING;
    } else if (circle->gameObject->y + circle->radius > 1.0f) {
        circle->gameObject->y = 1.0f - circle->radius;
        circle->gameObject->vy = -circle->gameObject->vy * BOUNCE_DAMPING;
    }
}
// void resolve_circle_collision(GameObject* obj1, GameObject* obj2) {
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
