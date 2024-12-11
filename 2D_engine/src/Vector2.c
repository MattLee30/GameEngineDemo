#include <math.h>
#include "headers/Vector2.h"

Vector2 vector_subtract(Vector2 a, Vector2 b) {
    return (Vector2){a.x - b.x, a.y - b.y};
}

float vector_dot(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

Vector2 vector_normalize(Vector2 v) {
    float length = sqrt(v.x * v.x + v.y * v.y);
    return (Vector2){v.x / length, v.y / length};
}

Vector2 vector_scale(Vector2 v, float s) {
    return (Vector2){v.x * s, v.y * s};
}

float vector_length(Vector2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}
