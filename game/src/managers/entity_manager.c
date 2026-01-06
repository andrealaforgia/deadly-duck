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

void initialize_all_entities(game_ptr game) {
    // Initialize duck in the center, right on top of the lake
    const int duck_height = SPRITE_DUCK_NORMAL.h * 2;  // Duck sprite height * 2x scale

    duck_init(&game->duck, LOGICAL_WIDTH / 2.0f, LAKE_START_Y - duck_height);

    // Initialize popcorn pool (all inactive)
    popcorn_pool_init(game->popcorn, MAX_POPCORN);

    // Initialize crabs in random positions in top 60% of screen
    const int top_60_percent = (int)(LOGICAL_HEIGHT * 0.6f);
    const int crab_width = 17 * 2;
    const int crab_height = 15 * 2;

    for (int i = 0; i < NUM_CRABS; i++) {
        // Random x position within screen bounds
        game->crabs[i].x = (float)(rand() % (LOGICAL_WIDTH - crab_width));

        // Random y position in top 60% of screen
        game->crabs[i].y = (float)(rand() % (top_60_percent - crab_height));

        // Random velocity between 1.5 and 3.75 pixels per frame
        float speed = 1.5f + ((float)rand() / RAND_MAX) * 2.25f;

        // Random initial direction
        game->crabs[i].moving_right = (rand() % 2) == 0;
        game->crabs[i].vx = game->crabs[i].moving_right ? speed : -speed;
        game->crabs[i].alive = true;  // All crabs start alive
        game->crabs[i].has_brick = false;
        game->crabs[i].off_screen = false;
        game->crabs[i].dropping = false;
        game->crabs[i].drop_start_time = 0;
        // Random time between 3-8 seconds before first drop
        game->crabs[i].next_drop_time = get_clock_ticks_ms() + 3000 + (rand() % 5000);
    }

    // Initialize bricks pool (all inactive)
    bricks_pool_init(game->bricks, MAX_BRICKS);

    // Initialize jellyfish midway between crabs and duck
    // Duck is at lake surface, crabs are in top 60%
    // Jellyfish should be around 70-75% down the screen
    int jellyfish_zone_y = (int)(LOGICAL_HEIGHT * 0.7f);

    // Jellyfish sprite dimensions: ~16 width, ~13 height (at 2x scale = 32x26)
    const int jellyfish_spacing = 1;  // 1 pixel space between jellyfish

    // Random fast velocity between 3.0 and 5.0 pixels per frame (same for all)
    float speed = 3.0f + ((float)rand() / RAND_MAX) * 2.0f;
    bool moving_right = (rand() % 2) == 0;  // Random initial direction

    // Calculate starting position to center all 4 jellyfish as a group
    // Total width = 4 jellyfish + 3 gaps
    float total_width = (16 * 2 * 4) + (jellyfish_spacing * 3);
    float start_x = (LOGICAL_WIDTH - total_width) / 2.0f;

    for (int i = 0; i < NUM_JELLYFISH; i++) {
        // Position horizontally: side by side, centered as a group
        game->jellyfish[i].x = start_x + (i * (16 * 2 + jellyfish_spacing));
        game->jellyfish[i].y = jellyfish_zone_y;

        // All jellyfish move in the same direction at the same speed
        game->jellyfish[i].moving_right = moving_right;
        game->jellyfish[i].vx = moving_right ? speed : -speed;

        game->jellyfish[i].anim_frame = i % 4;  // Stagger animation frames
        game->jellyfish[i].last_anim_time = get_clock_ticks_ms();
    }
}
