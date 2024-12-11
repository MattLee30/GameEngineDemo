// drawShape.c

#include <GLFW/glfw3.h>
#include <math.h>
#include "headers/drawShape.h"
#include "headers/gameObject.h"
#include "headers/input.h"
#include "headers/structs.h"


extern ShapeType selectedShapeType;

extern int paused;

void drawTriangle(float x, float y, float size);

void renderSelectionBar(GLFWwindow* window, int fbWidth, int fbHeight) {
    // Set viewport for selection bar (left 20% of the window)
    int topBarHeight = 125;
    int sbWidth = fbWidth * 0.2;
    int sbHeight = fbHeight - topBarHeight;
    glViewport(0, 100, sbWidth, sbHeight); 
    // Set orthographic projection for selection bar
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, sbWidth, 0, sbHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw background
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(sbWidth, 0);
    glVertex2f(sbWidth, fbHeight);
    glVertex2f(0, fbHeight);
    glEnd();
    float buttonHeight = 100.0f;
    float padding = 20.0f;
    float yPos = sbHeight - buttonHeight - padding - topBarHeight; // Start immediately below the top bar

    // Circle Button
    glColor3f(selectedShapeType == SHAPE_CIRCLE ? 0.7f : 0.5f, 0.5f, 0.5f); // Highlight based on selection
    glBegin(GL_QUADS);
    glVertex2f(padding, yPos);
    glVertex2f(sbWidth - padding, yPos);
    glVertex2f(sbWidth - padding, yPos + buttonHeight);
    glVertex2f(padding, yPos + buttonHeight);
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f); // Red circle icon
    drawCircle(sbWidth / 2.0f, yPos + buttonHeight / 2.0f, 30.0f, 30);

    // Square Button
    yPos -= (buttonHeight + padding); // Move to the next button position
    glColor3f(selectedShapeType == SHAPE_SQUARE ? 0.7f : 0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(padding, yPos);
    glVertex2f(sbWidth - padding, yPos);
    glVertex2f(sbWidth - padding, yPos + buttonHeight);
    glVertex2f(padding, yPos + buttonHeight);
    glEnd();

    glColor3f(0.0f, 0.0f, 1.0f); // Blue square icon
    drawSquare(sbWidth / 2.0f, yPos + buttonHeight / 2.0f, 60.0f);
}

void renderTopBar(GLFWwindow* window, int fbWidth, int fbHeight) {
    // Set viewport for top bar (top part of the window)
    int topBarHeight = 100; // Height of the top bar
    glViewport(0, fbHeight - topBarHeight, fbWidth, topBarHeight);

    // Set orthographic projection for the top bar
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, fbWidth, 0, topBarHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw background
    glColor3f(0.7f, 0.7f, 0.7f); // Lighter background for the top bar
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(fbWidth, 0);
    glVertex2f(fbWidth, topBarHeight);
    glVertex2f(0, topBarHeight);
    glEnd();

    // Draw Pause Button
    float padding = 20.0f;
    float buttonSize = 60.0f;
    // float xPos = fbWidth - buttonSize - padding;
    float xPos = 50.0f;
    float yPos = topBarHeight / 2.0f;

    glColor3f(paused ? 0.7f : 0.5f, 0.7f, 0.5f); // Green color if not paused
    glBegin(GL_QUADS);
    glVertex2f(xPos, yPos - buttonSize / 2.0f);
    glVertex2f(xPos + buttonSize, yPos - buttonSize / 2.0f);
    glVertex2f(xPos + buttonSize, yPos + buttonSize / 2.0f);
    glVertex2f(xPos, yPos + buttonSize / 2.0f);
    glEnd();

    // Draw triangle icon for pause button
    glColor3f(0.0f, 1.0f, 0.0f); // Green for the triangle
    drawTriangle(xPos + buttonSize / 2.0f, yPos, buttonSize / 2.0f);
}


void renderGameArea(int fbWidth, int fbHeight, float deltaTime) {
    // Set viewport for game area (right 80% of the window)
    int gaWidth = fbWidth * 0.8;
    glViewport(fbWidth * 0.2, 0, gaWidth, fbHeight);

    // Set orthographic projection for game area
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = (float)fbHeight / (float)gaWidth;
    glOrtho(-1.0f, 1.0f, -1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Update game objects
    if (!paused) {
        for (int i = 0; i < gameObjectCount; i++) {
            if (isDraggingObject && i == selectedObjectIndex) continue;
            updateGameObject(&gameObjects[i], deltaTime);
        }
        handleGameObjectCollisions();
    }

    // Handle collisions
    handleGameObjectCollisions();

    // Draw game objects
    for (int i = 0; i < gameObjectCount; i++) {
        drawGameObject(&gameObjects[i]);
    }

    if (dragging) {
        drawGameObject(&previewObject);
    }
}

void drawCircle(float x, float y, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Center of circle
    for (int i = 0; i <= segments; i++) {
        float angle = i * 2.0f * M_PI / segments;
        float dx = radius * cosf(angle);
        float dy = radius * sinf(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void drawSquare(float x, float y, float size) {
    float halfSize = size / 2.0f;
    glBegin(GL_QUADS);
    glVertex2f(x - halfSize, y - halfSize);
    glVertex2f(x + halfSize, y - halfSize);
    glVertex2f(x + halfSize, y + halfSize);
    glVertex2f(x - halfSize, y + halfSize);
    glEnd();
}

void drawTriangle(float x, float y, float size) {
    float halfSize = size / 2.0f;
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + halfSize);
    glVertex2f(x - halfSize, y - halfSize);
    glVertex2f(x + halfSize, y - halfSize);
    glEnd();
}
