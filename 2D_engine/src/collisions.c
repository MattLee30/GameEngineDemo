#include <math.h>
#include <stdio.h>
#include "headers/constants.h"
#include "headers/circle.h"
#include "headers/square.h"
#include "headers/Vector2.h"
#include <stdbool.h>


extern GameObject* gameObject;

float circle_circle_overlap(GameObject* obj1, GameObject* obj2);
float square_square_overlap(GameObject* obj1, GameObject* obj2);
float circle_square_overlap(GameObject* obj1, GameObject* obj2);

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
        // Push objects apart to resolve overlap
        float correction_factor = 1.0f; // Adjust this to fine-tune separation behavior
        Vector2 correction = vector_scale(normal, overlap * correction_factor);
        obj1->x -= correction.x * 0.5f; // Move obj1 half the correction distance
        obj1->y -= correction.y * 0.5f;
        obj2->x += correction.x * 0.5f; // Move obj2 half the correction distance
        obj2->y += correction.y * 0.5f;
    }

    // Calculate relative velocity
    Vector2 relative_velocity = vector_subtract((Vector2){obj1->vx, obj1->vy}, (Vector2){obj2->vx, obj2->vy});

    // Project relative velocity onto the collision normal
    float relative_speed = vector_dot(relative_velocity, normal);

    // If objects are already separating, no impulse is needed
    if (relative_speed >= 0 && overlap <= 0) {
        return;  // Skip only if they are separating AND there's no overlap
    }

    // Coefficient of restitution (elasticity factor, 0 = inelastic, 1 = elastic)
    float restitution = 0.8f;

    // Calculate the impulse scalar
    float impulse_scalar = -(1 + restitution) * relative_speed;
    float mass1 = 1.0f; // Assuming unit mass for simplicity
    float mass2 = 1.0f;
    impulse_scalar /= (1 / mass1 + 1 / mass2);

    // Apply the impulse to both objects
    Vector2 impulse = vector_scale(normal, impulse_scalar);
    obj1->vx += impulse.x / mass1;
    obj1->vy += impulse.y / mass1;
    obj2->vx -= impulse.x / mass2;
    obj2->vy -= impulse.y / mass2;
}


// Calculate the overlap between two circles
float circle_circle_overlap(GameObject* obj1, GameObject* obj2) {
    // Get the positions and radii of the circles
    float dx = obj2->x - obj1->x;
    float dy = obj2->y - obj1->y;
    float distance_squared = dx * dx + dy * dy;
    
    float radius_sum = obj1->shape.circle.radius + obj2->shape.circle.radius;
    float radius_sum_squared = radius_sum * radius_sum;

    // If the circles are colliding, return the amount of overlap
    if (distance_squared < radius_sum_squared) {
        float distance = sqrt(distance_squared);
        return radius_sum - distance;  // Overlap is the difference between radii and distance
    }
    return 0.0f; // No overlap
}

// Calculate the overlap between two squares
float square_square_overlap(GameObject* obj1, GameObject* obj2) {
    float half_size1 = obj1->shape.square.size / 2;
    float half_size2 = obj2->shape.square.size / 2;

    // Get the edges of the squares
    float left1 = obj1->x - half_size1;
    float right1 = obj1->x + half_size1;
    float top1 = obj1->y - half_size1;
    float bottom1 = obj1->y + half_size1;

    float left2 = obj2->x - half_size2;
    float right2 = obj2->x + half_size2;
    float top2 = obj2->y - half_size2;
    float bottom2 = obj2->y + half_size2;

    // Calculate the overlap in the X and Y directions
    float overlapX = fmin(right1, right2) - fmax(left1, left2);
    float overlapY = fmin(bottom1, bottom2) - fmax(top1, top2);

    // If there is overlap in both directions, return the minimum overlap
    if (overlapX > 0 && overlapY > 0) {
        return fmin(overlapX, overlapY);
    }

    return 0.0f; // No overlap
}

// Calculate the overlap between a circle and a square
float circle_square_overlap(GameObject* obj1, GameObject* obj2) {
    Circle* circle = &obj1->shape.circle;
    Square* square = &obj2->shape.square;

    float half_size = square->size / 2;

    // Find the closest point on the square to the circle's center
    float closestX = fmax(square->gameObject->x - half_size, 
                          fmin(circle->gameObject->x, square->gameObject->x + half_size));
    float closestY = fmax(square->gameObject->y - half_size, 
                          fmin(circle->gameObject->y, square->gameObject->y + half_size));

    // Calculate the distance from the circle's center to this closest point
    float dx = circle->gameObject->x - closestX;
    float dy = circle->gameObject->y - closestY;
    float distance = sqrt(dx * dx + dy * dy);

    // If the distance is less than the circle's radius, a collision occurs
    if (distance < circle->radius) {
        return circle->radius - distance;  // Return the overlap distance
    }

    return 0.0f; // No overlap
}
