#include <ft2build.h>
#include FT_FREETYPE_H
#include "headers/Vector2.h"
#include <OpenGL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "headers/textrenderer.h"

void initFreeType(FT_Library* ft, FT_Face* face, const char* fontPath) {
    if (FT_Init_FreeType(ft)) {
        fprintf(stderr, "Could not initialize FreeType library\n");
        exit(1);
    }

    if (FT_New_Face(*ft, fontPath, 0, face)) {
        fprintf(stderr, "Failed to load font: %s\n", fontPath);
        exit(1);
    }

    FT_Set_Pixel_Sizes(*face, 0, 48);
}

extern Character characters[128];

void loadGlyphs(FT_Face face, Character characters[128]) {
    unsigned int glyphTextures[128]; // Store textures for ASCII characters
    glGenTextures(128, glyphTextures);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "Failed to load glyph for char '%c'\n", c);
            continue;
        }

        FT_GlyphSlot g = face->glyph;

        glBindTexture(GL_TEXTURE_2D, glyphTextures[c]);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            g->bitmap.width,
            g->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            g->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character info (advance, size, offset)
        characters[c].advance = g->advance.x >> 6; // Advance width
        characters[c].size = (Vector2){g->bitmap.width, g->bitmap.rows};
        characters[c].bearing = (Vector2){g->bitmap_left, g->bitmap_top};
    }
}
