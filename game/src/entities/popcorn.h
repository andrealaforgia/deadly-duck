/**
 * @file popcorn.h
 * @brief Popcorn entity
 */

#ifndef GAME_ENTITIES_POPCORN_H_
#define GAME_ENTITIES_POPCORN_H_

#include "object_pool.h"
#include <stdbool.h>

/**
 * Popcorn entity structure
 */
typedef struct {
    float x;        // X position
    float y;        // Y position
    float vy;       // Velocity Y (vertical movement, can be negative if reflected)
    bool active;    // True if this popcorn is in flight
    bool reflected; // True if reflected by jellyfish
} popcorn_t;

// Pointer typedef for popcorn
typedef popcorn_t *popcorn_ptr;

// Popcorn sprite dimensions
#define POPCORN_WIDTH 7    // Sprite width
#define POPCORN_HEIGHT 6   // Sprite height
#define POPCORN_SPEED 6.0f // Upward speed

// Pool size
#define MAX_POPCORN 10 // Maximum number of popcorn on screen

/**
 * Spawn a popcorn using object pool
 *
 * @param pool Object pool for popcorn
 * @param x Starting X position
 * @param y Starting Y position
 * @return true if spawned successfully, false if pool is full
 */
bool popcorn_spawn(object_pool_t *pool, float x, float y);

/**
 * Update all active popcorn using object pool
 *
 * @param pool Object pool for popcorn
 * @param logical_height Screen height for bounds checking
 */
void popcorn_update_all(object_pool_t *pool, int logical_height);

/**
 * Reflect a popcorn downward
 *
 * @param popcorn Popcorn to reflect
 */
void popcorn_reflect(popcorn_ptr popcorn);

#endif // GAME_ENTITIES_POPCORN_H_
