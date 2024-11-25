#include <GLFW/glfw3.h>
#include <stdlib.h>

// Maximum number of points to store
#define MAX_POINTS 100000

typedef struct {
    float x, y;
} Point;

// Array to store points
static Point points[MAX_POINTS];
static int pointCount = 0;
static int isDrawing = 0;
static GLFWwindow* drawingWindow = NULL;

// Mouse button callback for drawing
void drawingMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (window != drawingWindow) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            isDrawing = 1;
        } else if (action == GLFW_RELEASE) {
            isDrawing = 0;
        }
    }
}

// Cursor position callback for drawing
void drawingCursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (window != drawingWindow) return;

    if (isDrawing && pointCount < MAX_POINTS) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float gl_x = (float)((xpos / width) * 2.0f - 1.0f);
        float gl_y = (float)(1.0f - (ypos / height) * 2.0f);

        points[pointCount].x = gl_x;
        points[pointCount].y = gl_y;
        pointCount++;
    }
}


// Framebuffer size callback for drawing window
void drawingFramebufferSizeCallback(GLFWwindow* window, int fbWidth, int fbHeight) {
    if (window != drawingWindow) return;

    glViewport(0, 0, fbWidth, fbHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = (float)fbWidth / (float)fbHeight;

    if (aspectRatio >= 1.0f) {
        glOrtho(-aspectRatio, aspectRatio, -1, 1, -1, 1);
    } else {
        glOrtho(-1, 1, -1 / aspectRatio, 1 / aspectRatio, -1, 1);
    }

    glMatrixMode(GL_MODELVIEW);
}



void initializeDrawing(GLFWwindow* window) {
    drawingWindow = window;

    glfwSetFramebufferSizeCallback(window, drawingFramebufferSizeCallback);
    glfwSetMouseButtonCallback(window, drawingMouseButtonCallback);
    glfwSetCursorPosCallback(window, drawingCursorPositionCallback);

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    drawingFramebufferSizeCallback(window, fbWidth, fbHeight);
}


// Render the drawing
void renderDrawing() {
    if (!drawingWindow) return;

    glClear(GL_COLOR_BUFFER_BIT);

    if (pointCount > 1) {
        glColor3f(1.0f, 0.0f, 0.0f); // Set drawing color (e.g., red)
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < pointCount; i++) {
            glVertex2f(points[i].x, points[i].y);
        }
        glEnd();
    }
}
