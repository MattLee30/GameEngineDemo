#ifndef CIRCLE_H
#define CIRCLE_H

#include "structs.h"
#include <GLFW/glfw3.h>
#include "circle.h"
#include <math.h>
#define PI 3.14159265358979323846

extern Circle* balls;
extern int ballCount;

void updateBall(Circle* ball, float deltaTime, float gravity) {
    ball->vy += gravity * deltaTime;

    ball->x += ball->vx * deltaTime;
    ball->y += ball->vy * deltaTime;

    float screenWidth = 800.0f;
    float screenHeight = 600.0f;

    // Check collision with left and right boundaries
    if (ball->x - ball->radius < 0.0f) {
        ball->x = ball->radius; 
        ball->vx = -ball->vx;   
    } else if (ball->x + ball->radius > screenWidth) {
        ball->x = screenWidth - ball->radius;
        ball->vx = -ball->vx;
    }

    // Check collision with the ground
    if (ball->y - ball->radius < 0.0f) {
        ball->y = ball->radius;
        ball->vy = -ball->vy;   
        ball->vy *= 0.8f;      
    } else if (ball->y + ball->radius > screenHeight) {
        ball->y = screenHeight - ball->radius;
        ball->vy = -ball->vy;
        ball->vy *= 0.8f;     
    }
}


void handleCollisions() {
    // Handle collisions between balls
    for (int i = 0; i < ballCount; i++) {
        Circle* ballA = &balls[i];

        for (int j = i + 1; j < ballCount; j++) {
            Circle* ballB = &balls[j];

            // Calculate the distance between the two balls
            float dx = ballB->x - ballA->x;
            float dy = ballB->y - ballA->y;
            float distance = sqrtf(dx * dx + dy * dy);

            // Check if they collide
            if (distance < ballA->radius + ballB->radius) {
                // Resolve collision (basic elastic collision)
                float overlap = (ballA->radius + ballB->radius) - distance;

                // Normalize the collision vector
                float nx = dx / distance;
                float ny = dy / distance;

                // Push balls apart
                ballA->x -= nx * overlap / 2.0f;
                ballA->y -= ny * overlap / 2.0f;
                ballB->x += nx * overlap / 2.0f;
                ballB->y += ny * overlap / 2.0f;

                // Reflect velocities (simplified elastic collision)
                float dotProduct = (ballA->vx - ballB->vx) * nx + (ballA->vy - ballB->vy) * ny;

                ballA->vx -= 2.0f * dotProduct * nx;
                ballA->vy -= 2.0f * dotProduct * ny;
                ballB->vx += 2.0f * dotProduct * nx;
                ballB->vy += 2.0f * dotProduct * ny;
            }
        }
    }
}


void drawCircle(float x, float y, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);  // Center
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * PI * i / segments;
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}
#endif // CIRCLE_H
