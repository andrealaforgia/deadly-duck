/**
 * @file crab.h
 * @brief Crab enemy entity
 */

#ifndef GAME_ENTITIES_CRAB_H_
#define GAME_ENTITIES_CRAB_H_

#include "object_pool.h"
#include "types.h"
#include <stdbool.h>

#include "brick.h"

/**
 * Crab enemy structure
 */
typedef struct {
    float x;                        // X position
    float y;                        // Y position
    float vx;                       // Velocity X (horizontal movement)
    bool moving_right;              // True if moving right, false if moving left
    bool alive;                     // True if crab is alive, false if hit
    bool has_brick;                 // True if crab is carrying a brick
    bool off_screen;                // True if crab has gone off screen
    bool dropping;                  // True if crab is currently dropping brick
    timestamp_ms_t drop_start_time; // When brick drop animation started
    timestamp_ms_t next_drop_time;  // When crab will drop next brick
} crab_t;

// Pointer typedef for crab
typedef crab_t *crab_ptr;

// Crab sprite dimensions (2x scale)
#define CRAB_WIDTH (17 * 2)  // Crab sprite width at 2x scale
#define CRAB_HEIGHT (15 * 2) // Crab sprite height at 2x scale

// Pool size and limits
#define NUM_CRABS 10            // Number of crabs on screen
#define MAX_CRABS_WITH_BRICKS 6 // Maximum crabs carrying bricks at once

// Movement constants
#define CRAB_MIN_SPEED 1.5f    // Minimum crab movement speed
#define CRAB_SPEED_RANGE 2.25f // Additional speed range (max speed = min + range)

// Animation timing
#define DROP_ANIM_DURATION 100 // Dropping animation duration in ms

/**
 * Update all crabs using object pool (movement, animations, brick dropping)
 *
 * @param crab_pool Object pool for crabs
 * @param brick_pool Object pool for spawning dropped bricks
 * @param logical_width Screen width for bounds checking
 * @param current_time Current game time
 * @param play_sound_callback Callback to play brick drop sound
 * @param sound_context Audio context for sound callback
 */
void crabs_update_all(object_pool_t *crab_pool, object_pool_t *brick_pool, int logical_width,
                      timestamp_ms_t current_time, void (*play_sound_callback)(void *, int), void *sound_context);

#endif // GAME_ENTITIES_CRAB_H_
