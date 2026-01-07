/**
 * @file entity_manager.c
 * @brief Centralized entity initialization and management implementation
 */

#include "entity_manager.h"
#include "clock.h"

#include <stdlib.h>
#include "constants.h"
#include "sprite_atlas.h"
#include "duck.h"
#include "projectile.h"
#include "brick.h"
#include "crab.h"
#include "jellyfish.h"
#include "object_pool.h"

// Forward declarations for helper functions
static void initialize_crabs(game_ptr game);
static void initialize_jellyfish(game_ptr game);

void initialize_all_entities(game_ptr game) {
    // Initialize duck in the center, right on top of the lake
    const int duck_height = SPRITE_DUCK_NORMAL.h * 2;  // Duck sprite height * 2x scale
    duck_init(&game->duck, LOGICAL_WIDTH / 2.0f, LAKE_START_Y - duck_height);

    // Initialize object pools
    game->popcorn_pool = create_object_pool(sizeof(popcorn_t), MAX_POPCORN);
    game->crab_pool = create_object_pool(sizeof(crab_t), NUM_CRABS);
    game->brick_pool = create_object_pool(sizeof(brick_t), MAX_BRICKS);
    game->jellyfish_pool = create_object_pool(sizeof(jellyfish_t), NUM_JELLYFISH);

    // Initialize crabs using object pool
    initialize_crabs(game);

    // Initialize jellyfish using object pool
    initialize_jellyfish(game);
}

static void initialize_crabs(game_ptr game) {
    // Initialize crabs in random positions in top 60% of screen
    const int top_60_percent = (int)(LOGICAL_HEIGHT * 0.6f);
    const int crab_width = 17 * 2;
    const int crab_height = 15 * 2;

    for (int i = 0; i < NUM_CRABS; i++) {
        size_t crab_index;
        crab_ptr crab = (crab_ptr)pool_acquire(&game->crab_pool, &crab_index);
        if (!crab) break;  // Pool is full

        // Random x position within screen bounds
        crab->x = (float)(rand() % (LOGICAL_WIDTH - crab_width));

        // Random y position in top 60% of screen
        crab->y = (float)(rand() % (top_60_percent - crab_height));

        // Random velocity between min and max speed
        float speed = CRAB_MIN_SPEED + ((float)rand() / RAND_MAX) * CRAB_SPEED_RANGE;

        // Random initial direction
        crab->moving_right = (rand() % 2) == 0;
        crab->vx = crab->moving_right ? speed : -speed;
        crab->alive = true;  // All crabs start alive
        crab->has_brick = false;
        crab->off_screen = false;
        crab->dropping = false;
        crab->drop_start_time = 0;
        // Random time between 3-8 seconds before first drop
        crab->next_drop_time = get_clock_ticks_ms() + 3000 + (rand() % 5000);
    }
}

static void initialize_jellyfish(game_ptr game) {
    // Initialize jellyfish midway between crabs and duck
    // Duck is at lake surface, crabs are in top 60%
    // Jellyfish should be around 70-75% down the screen
    int jellyfish_zone_y = (int)(LOGICAL_HEIGHT * 0.7f);

    // Jellyfish sprite dimensions: ~16 width, ~13 height (at 2x scale = 32x26)
    const int jellyfish_spacing = 1;  // 1 pixel space between jellyfish

    // Random fast velocity between min and max speed (same for all)
    float speed = JELLYFISH_MIN_SPEED + ((float)rand() / RAND_MAX) * JELLYFISH_SPEED_RANGE;
    bool moving_right = (rand() % 2) == 0;  // Random initial direction

    // Calculate starting position to center all 4 jellyfish as a group
    // Total width = 4 jellyfish + 3 gaps
    float total_width = (16 * 2 * 4) + (jellyfish_spacing * 3);
    float start_x = (LOGICAL_WIDTH - total_width) / 2.0f;

    for (int i = 0; i < NUM_JELLYFISH; i++) {
        size_t jellyfish_index;
        jellyfish_ptr jellyfish = (jellyfish_ptr)pool_acquire(&game->jellyfish_pool, &jellyfish_index);
        if (!jellyfish) break;  // Pool is full

        // Position horizontally: side by side, centered as a group
        jellyfish->x = start_x + (i * (16 * 2 + jellyfish_spacing));
        jellyfish->y = jellyfish_zone_y;

        // All jellyfish move in the same direction at the same speed
        jellyfish->moving_right = moving_right;
        jellyfish->vx = moving_right ? speed : -speed;

        jellyfish->anim_frame = i % 4;  // Stagger animation frames
        jellyfish->last_anim_time = get_clock_ticks_ms();
    }
}

void cleanup_all_entities(game_ptr game) {
    // Clean up object pools
    pool_destroy(&game->popcorn_pool);
    pool_destroy(&game->crab_pool);
    pool_destroy(&game->brick_pool);
    pool_destroy(&game->jellyfish_pool);
}
