#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum {
    SHAPE_CIRCLE,
    SHAPE_SQUARE
} ShapeType;

typedef struct {
    float x, y;
    float radius;
    float vx, vy;
    float r, g, b;
} Circle;

typedef struct {
    float x, y;
    float size;
    float vx, vy;
    float r, g, b;
} Square;

typedef union {
    Circle circle;
    Square square;
} ShapeUnion;

typedef struct {
    ShapeType type;
    ShapeUnion shape;
} GameObject;

#endif
