/**
 * @file brick.c
 * @brief Brick entity implementation
 */

#include "brick.h"
#include "types.h"

void bricks_pool_init(brick_t* bricks, int count) {
    for (int i = 0; i < count; i++) {
        bricks[i].active = false;
        bricks[i].landed = false;
        bricks[i].x = 0.0f;
        bricks[i].y = 0.0f;
        bricks[i].land_time = 0;
    }
}

bool brick_spawn(brick_t* bricks, int count, float x, float y) {
    for (int i = 0; i < count; i++) {
        if (!bricks[i].active) {
            bricks[i].active = true;
            bricks[i].landed = false;
            bricks[i].x = x;
            bricks[i].y = y;
            bricks[i].land_time = 0;
            return true;
        }
    }
    return false;  // Pool is full
}

void bricks_update_all(brick_t* bricks, int count, int lake_start_y, timestamp_ms_t current_time) {
    for (int i = 0; i < count; i++) {
        if (bricks[i].active && !bricks[i].landed) {
            // Fall downward
            bricks[i].y += BRICK_FALL_SPEED;

            // Check if brick reaches lake surface
            if (bricks[i].y + BRICK_HEIGHT >= lake_start_y) {
                bricks[i].landed = true;
                bricks[i].y = lake_start_y - BRICK_HEIGHT;  // Position on lake surface
                bricks[i].land_time = current_time;
            }
        } else if (bricks[i].landed) {
            // Check if timeout has passed
            if (current_time - bricks[i].land_time >= BRICK_LAND_DURATION) {
                bricks[i].active = false;
                bricks[i].landed = false;
            }
        }
    }
}
