/**
 * @file entity_factory.c
 * @brief Implementation of entity creation factory
 */

#include "entity_factory.h"
#include "clock.h"
#include "constants.h"
#include "object_pool.h"

#include <stdlib.h>

void create_duck(duck_ptr duck, float x, float y) { duck_init(duck, x, y); }

bool create_crab(crab_ptr crab) {
    if (!crab) {
        return false;
    }

    // Random position in top 60% of screen
    const int top_60_percent = (int)(LOGICAL_HEIGHT * 0.6f);
    const int crab_width = CRAB_WIDTH;
    const int crab_height = CRAB_HEIGHT;

    // Random x position within screen bounds
    crab->x = (float)(rand() % (LOGICAL_WIDTH - crab_width));

    // Random y position in top 60% of screen
    crab->y = (float)(rand() % (top_60_percent - crab_height));

    // Random velocity between min and max speed
    float speed = CRAB_MIN_SPEED + ((float)rand() / RAND_MAX) * CRAB_SPEED_RANGE;

    // Random initial direction
    crab->moving_right = (rand() % 2) == 0;
    crab->vx = crab->moving_right ? speed : -speed;
    crab->alive = true;
    crab->has_brick = false;
    crab->off_screen = false;
    crab->dropping = false;
    crab->drop_start_time = 0;

    // Random time between 3-8 seconds before first drop
    crab->next_drop_time = get_clock_ticks_ms() + 3000 + (rand() % 5000);

    return true;
}

void create_jellyfish(jellyfish_ptr jellyfish, float x, float y, float group_velocity_x, bool moving_right,
                      int anim_offset) {
    if (!jellyfish) {
        return;
    }

    jellyfish->x = x;
    jellyfish->y = y;
    jellyfish->moving_right = moving_right;
    jellyfish->vx = group_velocity_x;
    jellyfish->anim_frame = anim_offset % 4;
    jellyfish->last_anim_time = get_clock_ticks_ms();
}

void create_entity_pools(game_ptr game) {
    if (!game) {
        return;
    }

    game->popcorn_pool = create_object_pool(sizeof(popcorn_t), MAX_POPCORN);
    game->crab_pool = create_object_pool(sizeof(crab_t), NUM_CRABS);
    game->brick_pool = create_object_pool(sizeof(brick_t), MAX_BRICKS);
    game->jellyfish_pool = create_object_pool(sizeof(jellyfish_t), NUM_JELLYFISH);
}

void destroy_entity_pools(game_ptr game) {
    if (!game) {
        return;
    }

    pool_destroy(&game->popcorn_pool);
    pool_destroy(&game->crab_pool);
    pool_destroy(&game->brick_pool);
    pool_destroy(&game->jellyfish_pool);
}
