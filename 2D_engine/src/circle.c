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
    circle->gameObject->x += circle->gameObject->vx * deltaTime;
    circle->gameObject->y += circle->gameObject->vy * deltaTime;

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