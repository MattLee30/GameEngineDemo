#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

#define PI 3.14159265358979323846
#define GRAVITY -5.8f  
#define BOUNCE_DAMPING 0.8f 

typedef struct {
    float x, y;       
    float radius;   
    float velocityX;
    float velocityY; 
} Circle;

Circle* balls = NULL;   
int ballCount = 0;     
bool dragging = false; 
Circle previewBall;      

double dragStartX, dragStartY;
double dragStartTime;

// Function to draw a circle
void drawCircle(float x, float y, float radius, int segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float theta = 2.0f * PI * i / segments;
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

// Function to update ball position and handle wall collisions
void updateBall(Circle* ball, float deltaTime) {
    ball->velocityY += GRAVITY * deltaTime;
    ball->x += ball->velocityX * deltaTime;
    ball->y += ball->velocityY * deltaTime;

    // Bounce off walls (left and right)
    if (ball->x - ball->radius <= -1.0f) {
        ball->x = -1.0f + ball->radius;
        ball->velocityX = -ball->velocityX * BOUNCE_DAMPING;
    } else if (ball->x + ball->radius >= 1.0f) {
        ball->x = 1.0f - ball->radius;
        ball->velocityX = -ball->velocityX * BOUNCE_DAMPING;
    }

    // Bounce off floor
    if (ball->y - ball->radius <= -1.0f) {
        ball->y = -1.0f + ball->radius;
        ball->velocityY = -ball->velocityY * BOUNCE_DAMPING;
    }

    // Bounce off ceiling
    if (ball->y + ball->radius >= 1.0f) {
        ball->y = 1.0f - ball->radius;
        ball->velocityY = -ball->velocityY * BOUNCE_DAMPING;
    }

    // Damping to stop small movements
    if (fabs(ball->velocityY) < 0.0001f && ball->y - ball->radius <= -1.0f) {
        ball->velocityY = 0.0f;  // Stop the ball if it's bouncing too slowly
    }
    if (fabs(ball->velocityX) < 0.0001f) {
        ball->velocityX = 0.0f;
    }
}

// Function to handle collisions between balls
void handleCollisions() {
    for (int i = 0; i < ballCount; i++) {
        Circle* ballA = &balls[i];
        for (int j = i + 1; j < ballCount; j++) {
            Circle* ballB = &balls[j];

            float dx = ballB->x - ballA->x;
            float dy = ballB->y - ballA->y;
            float distance = sqrtf(dx * dx + dy * dy);
            float minDistance = ballA->radius + ballB->radius;

            if (distance < minDistance) {
                // Balls are colliding

                // Normal vector
                float nx = dx / distance;
                float ny = dy / distance;

                // Overlap amount
                float overlap = minDistance - distance;

                // Separate balls to avoid overlap
                ballA->x -= nx * overlap / 2.0f;
                ballA->y -= ny * overlap / 2.0f;
                ballB->x += nx * overlap / 2.0f;
                ballB->y += ny * overlap / 2.0f;

                // Relative velocity
                float vx = ballB->velocityX - ballA->velocityX;
                float vy = ballB->velocityY - ballA->velocityY;

                // Dot product of relative velocity and normal
                float dot = vx * nx + vy * ny;

                // Apply collision response if balls are moving towards each other
                if (dot < 0.0f) {
                    // Coefficient of restitution
                    float restitution = BOUNCE_DAMPING;

                    // Impulse scalar
                    float impulse = -(1.0f + restitution) * dot / 2.0f;  // Equal mass assumed

                    // Apply impulse to velocities
                    ballA->velocityX -= impulse * nx;
                    ballA->velocityY -= impulse * ny;
                    ballB->velocityX += impulse * nx;
                    ballB->velocityY += impulse * ny;
                }
            }
        }
    }
}

// Callback for resizing the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = (float)width / (float)height;
    if (aspectRatio >= 1.0f) {
        glOrtho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-1.0f, 1.0f, -1.0f / aspectRatio, 1.0f / aspectRatio, -1.0f, 1.0f);
    }
    glMatrixMode(GL_MODELVIEW);
}

// Mouse button callback function
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            double xpos, ypos;
            int width, height;

            // Get mouse position in screen coordinates
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwGetWindowSize(window, &width, &height);

            // Convert screen coordinates to OpenGL coordinates
            float gl_x = (float)((xpos / width) * 2.0f - 1.0f);
            float gl_y = (float)(1.0f - (ypos / height) * 2.0f);

            // Start dragging and create a preview ball
            dragging = true;
            previewBall = (Circle){gl_x, gl_y, 0.1f, 0.0f, 0.0f};

            dragStartX = gl_x;
            dragStartY = gl_y;
            dragStartTime = glfwGetTime();
        } else if (action == GLFW_RELEASE && dragging) {
            double xpos, ypos;
            int width, height;

            // Get mouse position in screen coordinates
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwGetWindowSize(window, &width, &height);

            // Convert screen coordinates to OpenGL coordinates
            float gl_x = (float)((xpos / width) * 2.0f - 1.0f);
            float gl_y = (float)(1.0f - (ypos / height) * 2.0f);

            double dragEndTime = glfwGetTime();
            double deltaTime = dragEndTime - dragStartTime;

            if (deltaTime > 0.0) {
                float velocityScale = 2.0f; // Adjust as needed
                float deltaX = gl_x - dragStartX;
                float deltaY = gl_y - dragStartY;
                float initialVelocityX = deltaX / deltaTime * velocityScale;
                float initialVelocityY = deltaY / deltaTime * velocityScale;

                // Create the new ball with initial velocities
                Circle newBall = previewBall;
                newBall.velocityX = initialVelocityX;
                newBall.velocityY = initialVelocityY;

                // Add new ball to array
                balls = (Circle*)realloc(balls, (ballCount + 1) * sizeof(Circle));
                balls[ballCount] = newBall;
                ballCount++;
            }

            dragging = false;
        }
    }
}

// Mouse move callback to update preview ball position while dragging
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (dragging) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Convert screen coordinates to OpenGL coordinates
        float gl_x = (float)((xpos / width) * 2.0f - 1.0f);
        float gl_y = (float)(1.0f - (ypos / height) * 2.0f);

        // Update the preview ball position
        previewBall.x = gl_x;
        previewBall.y = gl_y;
    }
}

int main() {
    if (!glfwInit()) {
        return -1;
    }

    // Create a window using GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "2D Basic", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set the framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Set the mouse button callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Set the cursor position callback
    glfwSetCursorPosCallback(window, cursor_position_callback);

    // Get initial window size and set viewport and projection matrix
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    framebuffer_size_callback(window, width, height);

    float lastTime = glfwGetTime();

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT);

        // Update all balls
        for (int i = 0; i < ballCount; i++) {
            updateBall(&balls[i], deltaTime);
        }

        // Handle collisions between balls
        handleCollisions();

        // Draw all balls
        for (int i = 0; i < ballCount; i++) {
            drawCircle(balls[i].x, balls[i].y, balls[i].radius, 100);
        }

        // If dragging, show the preview ball
        if (dragging) {
            drawCircle(previewBall.x, previewBall.y, previewBall.radius, 100);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    free(balls);

    // Clean up
    glfwTerminate();
    return 0;
}
