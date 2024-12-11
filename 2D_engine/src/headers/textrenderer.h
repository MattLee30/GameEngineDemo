#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Vector2.h"

typedef struct {
    float x, y, z; // Define Vector3 for color (RGB)
} Vector3;

// Define the Character structure for storing glyph data
typedef struct {
    unsigned int textureID;  // Texture ID for the glyph
    Vector2 size;            // Size of the glyph
    Vector2 bearing;         // Bearing offset for positioning
    unsigned int advance;    // Advance width for text spacing
} Character;

// Declare the global character array (defined elsewhere in a .c file)
extern Character characters[128];

// Function declarations
void initFreeType(FT_Library* ft, FT_Face* face, const char* fontPath);
void loadGlyphs(FT_Face face, Character characters[128]);
void renderText(const char* text, float x, float y, float scale, Vector3 color);

#endif // TEXTRENDERER_H
