#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct GameObject GameObject;

typedef enum {
    SHAPE_CIRCLE,
    SHAPE_SQUARE
} ShapeType;

typedef struct {
    float radius;
    GameObject* gameObject;
} Circle;

typedef struct {
    float size;
    GameObject* gameObject;
} Square;

typedef union {
    Circle circle;
    Square square;
} ShapeUnion;

typedef struct GameObject{
    ShapeType type;
    ShapeUnion shape;
    float x, y;
    float vx, vy;
    float r, g , b;
} GameObject;

#endif
