/**
 * @file brick.h
 * @brief Brick entity (dropped by crabs)
 */

#ifndef GAME_ENTITIES_BRICK_H_
#define GAME_ENTITIES_BRICK_H_

#include <stdbool.h>
#include "types.h"
#include "object_pool.h"

/**
 * Brick structure
 */
typedef struct {
    float x;           // X position
    float y;           // Y position
    bool active;       // True if brick is falling or landed
    bool landed;       // True if brick has landed on lake surface
    timestamp_ms_t land_time;  // When brick landed (for timeout)
} brick_t;

// Pointer typedef for brick
typedef brick_t* brick_ptr;

// Brick sprite dimensions
#define BRICK_WIDTH 13   // Sprite width
#define BRICK_HEIGHT 5   // Sprite height
#define BRICK_FALL_SPEED 6.0f  // Fall speed

// Pool size and timing
#define MAX_BRICKS 10   // Maximum number of falling bricks on screen
#define BRICK_LAND_DURATION 4000 // Bricks stay on lake for 4 seconds

/**
 * Spawn a falling brick using object pool
 *
 * @param pool Object pool for bricks
 * @param x Starting X position
 * @param y Starting Y position
 * @return true if spawned successfully, false if pool is full
 */
bool brick_spawn(object_pool_t* pool, float x, float y);

/**
 * Update all bricks using object pool (falling and landed timeout)
 *
 * @param pool Object pool for bricks
 * @param lake_start_y Y position of lake surface
 * @param current_time Current game time
 */
void bricks_update_all(object_pool_t* pool, int lake_start_y, timestamp_ms_t current_time);

#endif  // GAME_ENTITIES_BRICK_H_
