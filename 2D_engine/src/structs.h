#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    float x, y;       
    float radius;   
    float vx, vy; 
    float r, g, b; 
} Circle;

typedef struct {
    float x, y;       
    float sideLength; 
    float velocityX;
    float velocityY;
    float r, g, b; 
} Square;

// Enum for shape types
typedef enum {
    SHAPE_CIRCLE,
    SHAPE_SQUARE
} ShapeType;

typedef struct {
    ShapeType type; 
    union {
        Circle circle;  
        Square square;  
    } shape;
} GameObject;

#endif // STRUCTS_H