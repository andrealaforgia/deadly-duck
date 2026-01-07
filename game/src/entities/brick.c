/**
 * @file brick.c
 * @brief Brick entity implementation
 */

#include "brick.h"
#include "types.h"

bool brick_spawn(object_pool_t *pool, float x, float y) {
    size_t index;
    brick_ptr brick = (brick_ptr)pool_acquire(pool, &index);
    if (!brick) {
        return false; // Pool is full
    }

    brick->active = true;
    brick->landed = false;
    brick->x = x;
    brick->y = y;
    brick->land_time = 0;
    return true;
}

void bricks_update_all(object_pool_t *pool, int lake_start_y, timestamp_ms_t current_time) {
    // Manual iteration since we need to potentially release objects
    for (size_t i = 0; i < pool->capacity; i++) {
        if (!pool_is_active(pool, i))
            continue;

        brick_ptr brick = (brick_ptr)pool_get_at(pool, i);
        if (!brick || !brick->active)
            continue;

        if (!brick->landed) {
            // Fall downward
            brick->y += BRICK_FALL_SPEED;

            // Check if brick reaches lake surface
            if (brick->y + BRICK_HEIGHT >= lake_start_y) {
                brick->landed = true;
                brick->y = lake_start_y - BRICK_HEIGHT; // Position on lake surface
                brick->land_time = current_time;
            }
        } else {
            // Check if timeout has passed
            if (current_time - brick->land_time >= BRICK_LAND_DURATION) {
                brick->active = false;
                brick->landed = false;
                pool_release(pool, i);
            }
        }
    }
}
