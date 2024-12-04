#include "gameobject.h"

typedef void (*CollisionHandler)(GameObject* objA, GameObject* objB);

// Array to store collision handlers for each pair of object types
CollisionHandler collisionHandlers[NUM_SHAPE_TYPES][NUM_SHAPE_TYPES];

void initializeCollisionHandlers() {
    for (int i = 0; i < NUM_SHAPE_TYPES; ++i) {
        for (int j = 0; j < NUM_SHAPE_TYPES; ++j) {
            collisionHandlers[i][j] = NULL; // Default to no collision handler
        }
    }

    // Register specific handlers
    collisionHandlers[SHAPE_CIRCLE][SHAPE_CIRCLE] = (CollisionHandler)handleCollisions;
    collisionHandlers[SHAPE_SQUARE][SHAPE_SQUARE] = (CollisionHandler)handleSquareCollisions;
    // Add other combinations as needed
}
