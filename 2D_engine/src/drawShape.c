// drawShape.c

#include <GLFW/glfw3.h>
#include <math.h>
#include "headers/drawShape.h"
#include "headers/gameobject.h"
#include "headers/input.h"
#include "headers/structs.h"


extern ShapeType selectedShapeType;

void renderSelectionBar(int fbWidth, int fbHeight) {
    // Set viewport for selection bar (left 20% of the window)
    int sbWidth = fbWidth * 0.2;
    glViewport(0, 0, sbWidth, fbHeight);

    // Set orthographic projection for selection bar
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, sbWidth, 0, fbHeight, -1, 1);

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

    // Draw buttons
    float buttonHeight = 100.0f;
    float padding = 20.0f;
    float yPos = fbHeight - buttonHeight - padding;

    // Circle Button
    glColor3f(selectedShapeType == SHAPE_CIRCLE ? 0.7f : 0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(padding, yPos);
    glVertex2f(sbWidth - padding, yPos);
    glVertex2f(sbWidth - padding, yPos + buttonHeight);
    glVertex2f(padding, yPos + buttonHeight);
    glEnd();

    // Draw circle icon
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCircle(sbWidth / 2.0f, yPos + buttonHeight / 2.0f, 30.0f, 30);

    // Square Button
    yPos -= (buttonHeight + padding);
    glColor3f(selectedShapeType == SHAPE_SQUARE ? 0.7f : 0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(padding, yPos);
    glVertex2f(sbWidth - padding, yPos);
    glVertex2f(sbWidth - padding, yPos + buttonHeight);
    glVertex2f(padding, yPos + buttonHeight);
    glEnd();

    // Draw square icon
    glColor3f(0.0f, 0.0f, 1.0f);
    drawSquare(sbWidth / 2.0f, yPos + buttonHeight / 2.0f, 60.0f);
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

    // Update and render game objects
    // Update game objects
    for (int i = 0; i < gameObjectCount; i++) {
        if (isDraggingObject && i == selectedObjectIndex) continue;

        updateGameObject(&gameObjects[i], deltaTime);
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
