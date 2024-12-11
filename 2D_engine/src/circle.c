// circle.c

#include <math.h>
#include "headers/circle.h"
#include "headers/constants.h"
#include "headers/gameObject.h"
#include <stdio.h>

extern GameObject* gameObject;

void updateBall(Circle* circle, float deltaTime, float gravity) {
    if (!circle || !circle->gameObject) {
        fprintf(stderr, "Error: Circle or its GameObject is NULL.\n");
        return;
    }
    // Update position based on velocity and deltaTime
    circle->gameObject->x += circle->gameObject->vx * deltaTime;
    circle->gameObject->y += circle->gameObject->vy * deltaTime;

    // Apply gravity
    circle->gameObject->vy += gravity * deltaTime;

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

void handleCollisions(Circle* circleA, Circle* circleB) {
    float dx = circleB->gameObject->x - circleA->gameObject->x;
    float dy = circleB->gameObject->y - circleA->gameObject->y;
    float distance = sqrtf(dx * dx + dy * dy);
    float minDistance = circleA->radius + circleB->radius;

    if (distance < minDistance) {
        // Collision detected, resolve it
        float overlap = 0.5f * (minDistance - distance);

        // Avoid division by zero
        if (distance == 0.0f) {
            distance = 0.001f;
        }

        // Normalize the collision normal vector
        float nx = dx / distance;
        float ny = dy / distance;

        // Displace circles out of collision
        circleA->gameObject->x -= overlap * nx;
        circleA->gameObject->y -= overlap * ny;
        circleB->gameObject->x += overlap * nx;
        circleB->gameObject->y += overlap * ny;

        // Relative velocity
        float rvx = circleB->gameObject->vx - circleA->gameObject->vx;
        float rvy = circleB->gameObject->vy - circleA->gameObject->vy;

        // Calculate relative velocity along the normal
        float vn = rvx * nx + rvy * ny;

        // If velocities are separating, do nothing
        if (vn > 0.0f)
            return;

        // Coefficient of restitution (elasticity)
        float restitution = BOUNCE_DAMPING;

        // Impulse scalar
        float impulse = -(1.0f + restitution) * vn;
        impulse /= 2.0f; // Assuming mass = 1 for both circles

        // Apply impulse to the circles' velocities
        float impulseX = impulse * nx;
        float impulseY = impulse * ny;

        circleA->gameObject->vx -= impulseX;
        circleA->gameObject->vy -= impulseY;
        circleB->gameObject->vx += impulseX;
        circleB->gameObject->vy += impulseY;
    }
}
