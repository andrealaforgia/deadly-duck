/**
 * @file crab.h
 * @brief Crab enemy entity
 */

#ifndef GAME_ENTITIES_CRAB_H_
#define GAME_ENTITIES_CRAB_H_

#include <stdbool.h>
#include "types.h"

#include "brick.h"

/**
 * Crab enemy structure
 */
typedef struct {
    float x;           // X position
    float y;           // Y position
    float vx;          // Velocity X (horizontal movement)
    bool moving_right; // True if moving right, false if moving left
    bool alive;        // True if crab is alive, false if hit
    bool has_brick;    // True if crab is carrying a brick
    bool off_screen;   // True if crab has gone off screen
    bool dropping;     // True if crab is currently dropping brick
    timestamp_ms_t drop_start_time;  // When brick drop animation started
    timestamp_ms_t next_drop_time;   // When crab will drop next brick
} crab_t;

// Pointer typedef for crab
typedef crab_t* crab_ptr;

// Crab sprite dimensions (2x scale)
#define CRAB_WIDTH (17 * 2)   // Crab sprite width at 2x scale
#define CRAB_HEIGHT (15 * 2)  // Crab sprite height at 2x scale

// Pool size and limits
#define NUM_CRABS 10    // Number of crabs on screen
#define MAX_CRABS_WITH_BRICKS 6  // Maximum crabs carrying bricks at once

// Animation timing
#define DROP_ANIM_DURATION 100  // Dropping animation duration in ms

/**
 * Initialize all crabs with starting positions
 *
 * @param crabs Array of crabs
 * @param count Number of crabs in array
 * @param logical_width Screen width for positioning
 * @param y_position Y position for all crabs
 */
void crabs_init_all(crab_t* crabs, int count, int logical_width, float y_position);

/**
 * Update all crabs (movement, animations, brick dropping)
 *
 * @param crabs Array of crabs
 * @param count Number of crabs in array
 * @param bricks Brick pool for spawning dropped bricks
 * @param brick_count Number of bricks in pool
 * @param logical_width Screen width for bounds checking
 * @param current_time Current game time
 * @param play_sound_callback Callback to play brick drop sound
 * @param sound_context Audio context for sound callback
 */
void crabs_update_all(crab_t* crabs, int count, brick_t* bricks, int brick_count,
                      int logical_width, timestamp_ms_t current_time,
                      void (*play_sound_callback)(void*, int), void* sound_context);

#endif  // GAME_ENTITIES_CRAB_H_
