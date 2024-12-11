#include "headers/textrenderer.h"
#include <OpenGL/gl.h>   // For OpenGL functions and types
#include <GLFW/glfw3.h> // For the OpenGL context and window management
#include <stdio.h>     // For `fprintf` and `snprintf`
#include <stdlib.h>    // For `exit`
#include <string.h>    // For string manipulation

Character characters[128]; // Initialize the global characters array

void renderText(const char* text, float x, float y, float scale, Vector3 color) {
    glUseProgram(0); // Use default shaders if applicable

    glColor3f(color.x, color.y, color.z); // Set text color

    for (const char* p = text; *p; p++) {
        Character ch = characters[*p];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(xpos, ypos);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(xpos + w, ypos);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(xpos + w, ypos + h);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(xpos, ypos + h);
        glEnd();

        x += (ch.advance >> 6) * scale; // Advance cursor to next glyph
    }
}

