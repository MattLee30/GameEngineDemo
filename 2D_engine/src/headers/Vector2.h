
#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct {
    float x, y;
} Vector2;

extern Vector2 vector_subtract(Vector2 a, Vector2 b);
extern float vector_dot(Vector2 a, Vector2 b);
extern Vector2 vector_normalize(Vector2 v);
extern Vector2 vector_scale(Vector2 v, float s);
extern float vector_length(Vector2 v);

#endif // VECTOR2_H