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

bool circle_circle_collide(Circle* circle1, Circle* circle2) {
    float dx = circle2->gameObject->x - circle1->gameObject->x;
    float dy = circle2->gameObject->y - circle1->gameObject->y;
    float distance_squared = dx * dx + dy * dy;
    float radius_sum = circle1->radius + circle2->radius;
    return distance_squared < (radius_sum * radius_sum);
}

bool square_square_collide(Square* square1, Square* square2) {
    // Get the min and max x and y coordinates for both squares
    float left1 = square1->gameObject->x - square1->size / 2;
    float right1 = square1->gameObject->x + square1->size / 2;
    float top1 = square1->gameObject->y - square1->size / 2;
    float bottom1 = square1->gameObject->y + square1->size / 2;

    float left2 = square2->gameObject->x - square2->size / 2;
    float right2 = square2->gameObject->x + square2->size / 2;
    float top2 = square2->gameObject->y - square2->size / 2;
    float bottom2 = square2->gameObject->y + square2->size / 2;

    // Check for overlap in both x and y directions
    return !(right1 < left2 || left1 > right2 || bottom1 < top2 || top1 > bottom2);
}

void resolve_collision(GameObject* obj1, GameObject* obj2) {
    // Calculate normal vector between the two objects
    Vector2 normal = vector_subtract((Vector2){obj2->x, obj2->y}, (Vector2){obj1->x, obj1->y});
    float distance = vector_length(normal);

    if (distance == 0.0f) {
        // Avoid division by zero by setting a small displacement
        normal = (Vector2){1.0f, 0.0f};
        distance = 0.001f;
    } else {
        normal = vector_scale(normal, 1.0f / distance); // Normalize the normal vector
    }

    // Calculate the overlap distance
    float radius1 = obj1->shape.circle.radius;
    float radius2 = obj2->shape.circle.radius;
    float overlap = (radius1 + radius2) - distance;

    if (overlap > 0) {
        float correction_factor = 0.3f;
        Vector2 correction = vector_scale(normal, overlap * correction_factor);
        obj1->x -= correction.x * 0.5f; 
        obj1->y -= correction.y * 0.5f;
        obj2->x += correction.x * 0.5f; 
        obj2->y += correction.y * 0.5f;
    }

    // Calculate relative velocity
    Vector2 relative_velocity = vector_subtract((Vector2){obj1->vx, obj1->vy}, (Vector2){obj2->vx, obj2->vy});

    // Project relative velocity onto the collision normal
    float relative_speed = vector_dot(relative_velocity, normal);

    // If objects are already separating, no impulse is needed
    if (relative_speed >= 0 && overlap <= 0) {
        return;
    }

    // Coefficient of restitution (elasticity factor, 0 = inelastic, 1 = elastic)
    float restitution = 0.8f;

    float impulse_scalar = -(1 + restitution) * relative_speed;
    float mass1 = 1.0f;
    float mass2 = 1.0f;
    impulse_scalar /= (1 / mass1 + 1 / mass2);

    Vector2 impulse = vector_scale(normal, impulse_scalar);
    obj1->vx += impulse.x / mass1;
    obj1->vy += impulse.y / mass1;
    obj2->vx -= impulse.x / mass2;
    obj2->vy -= impulse.y / mass2;
}