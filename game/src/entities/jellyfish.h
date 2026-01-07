/**
 * @file jellyfish.h
 * @brief Jellyfish enemy entity
 */

#ifndef GAME_ENTITIES_JELLYFISH_H_
#define GAME_ENTITIES_JELLYFISH_H_

#include <stdbool.h>
#include "types.h"
#include "object_pool.h"

/**
 * Jellyfish enemy structure
 */
typedef struct {
    float x;           // X position
    float y;           // Y position
    float vx;          // Velocity X (horizontal movement)
    bool moving_right; // True if moving right, false if moving left
    int anim_frame;    // Current animation frame (0-3)
    timestamp_ms_t last_anim_time;  // Last animation frame change time
} jellyfish_t;

// Pointer typedef for jellyfish
typedef jellyfish_t* jellyfish_ptr;

// Jellyfish sprite dimensions (2x scale)
#define JELLYFISH_WIDTH (16 * 2)  // Wider frame at 2x scale
#define JELLYFISH_HEIGHT (13 * 2)  // Jellyfish sprite height at 2x scale

// Movement constants
#define JELLYFISH_MIN_SPEED 3.0f     // Minimum jellyfish movement speed  
#define JELLYFISH_SPEED_RANGE 2.0f   // Additional speed range (max speed = min + range)

// Animation constants
#define JELLYFISH_FRAME_COUNT 4      // Number of animation frames
#define ANIMATION_CYCLE_MS 250       // Time per animation frame in milliseconds

// Pool size
#define NUM_JELLYFISH 4  // Number of jellyfish on screen

/**
 * Update all jellyfish using object pool (movement and animation)
 * Note: All jellyfish move together and bounce together
 *
 * @param pool Object pool for jellyfish
 * @param logical_width Screen width for bounds checking
 * @param current_time Current game time for animation
 */
void jellyfish_update_all(object_pool_t* pool, int logical_width, timestamp_ms_t current_time);

#endif  // GAME_ENTITIES_JELLYFISH_H_
