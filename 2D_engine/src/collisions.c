#include <math.h>
#include <stdio.h>
#include "headers/constants.h"
#include "headers/circle.h"
#include "headers/square.h"
#include "headers/Vector2.h"
#include <stdbool.h>

extern GameObject* gameObject;

// Check for collision between circle and square
bool circle_square_collide(Circle* circle, Square* square) {
    float closestX = fmax(square->gameObject->x - square->size / 2, 
                          fmin(circle->gameObject->x, square->gameObject->x + square->size / 2));
    float closestY = fmax(square->gameObject->y - square->size / 2, 
                          fmin(circle->gameObject->y, square->gameObject->y + square->size / 2));

    // Calculate the distance from the circle's center to this closest point
    float dx = circle->gameObject->x - closestX;
    float dy = circle->gameObject->y - closestY;

    // If the distance is less than the circle's radius, a collision occurs
    return (dx * dx + dy * dy) < (circle->radius * circle->radius);
}

void resolve_collision(GameObject* obj1, GameObject* obj2) {
    // Calculate the normal of the collision (in this case, from the circle to the square)
    Vector2 normal = vector_subtract((Vector2){obj2->x, obj2->y}, (Vector2){obj1->x, obj1->y});
    normal = vector_normalize(normal);  // Normalize the normal vector

    // Calculate relative velocity between the two objects
    Vector2 relative_velocity = vector_subtract((Vector2){obj1->vx, obj1->vy}, 
                                                (Vector2){obj2->vx, obj2->vy});

    // Calculate the impulse scalar using the dot product of the relative velocity and normal
    float relative_speed = vector_dot(relative_velocity, normal);

    if (relative_speed > 0) {
        // If the objects are separating, no need to apply impulse
        return;
    }

    // Calculate the impulse based on masses (assuming both have mass = 1 for simplicity)
    float impulse = -(1 + 1) * relative_speed / (1 + 1); // Assume both objects have mass 1

    // Apply impulse to both objects
    Vector2 impulse_vector = vector_scale(normal, impulse);

    obj1->vx += impulse_vector.x;
    obj1->vy += impulse_vector.y;

    obj2->vx -= impulse_vector.x;
    obj2->vy -= impulse_vector.y;
}
