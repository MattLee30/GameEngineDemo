// circle.c

#include <math.h>
#include "headders/circle.h"
#include "headders/constants.h"


void updateBall(Circle* circle, float deltaTime, float gravity) {
    // Update position based on velocity and deltaTime
    circle->x += circle->vx * deltaTime;
    circle->y += circle->vy * deltaTime;

    // Apply gravity
    circle->vy += gravity * deltaTime;

    // Handle collisions with window boundaries
    if (circle->x - circle->radius < -1.0f) {
        circle->x = -1.0f + circle->radius;
        circle->vx = -circle->vx * BOUNCE_DAMPING;
    } else if (circle->x + circle->radius > 1.0f) {
        circle->x = 1.0f - circle->radius;
        circle->vx = -circle->vx * BOUNCE_DAMPING;
    }

    if (circle->y - circle->radius < -1.0f) {
        circle->y = -1.0f + circle->radius;
        circle->vy = -circle->vy * BOUNCE_DAMPING;
    } else if (circle->y + circle->radius > 1.0f) {
        circle->y = 1.0f - circle->radius;
        circle->vy = -circle->vy * BOUNCE_DAMPING;
    }
}

void handleCollisions(Circle* circleA, Circle* circleB) {
    float dx = circleB->x - circleA->x;
    float dy = circleB->y - circleA->y;
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
        circleA->x -= overlap * nx;
        circleA->y -= overlap * ny;
        circleB->x += overlap * nx;
        circleB->y += overlap * ny;

        // Relative velocity
        float rvx = circleB->vx - circleA->vx;
        float rvy = circleB->vy - circleA->vy;

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

        circleA->vx -= impulseX;
        circleA->vy -= impulseY;
        circleB->vx += impulseX;
        circleB->vy += impulseY;
    }
}
