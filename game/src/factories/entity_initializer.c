/**
 * @file entity_initializer.c
 * @brief Implementation of entity initialization using factory
 */

#include "entity_initializer.h"
#include "entity_factory.h"
#include "constants.h"
#include "clock.h"
#include "object_pool.h"

#include <stdlib.h>

// Forward declarations for helper functions
static void initialize_crabs(game_ptr game);
static void initialize_jellyfish(game_ptr game);

void initialize_all_entities(game_ptr game) {
    // Initialize duck in the center, right on top of the lake
    const int duck_height = DUCK_HEIGHT;
    create_duck(&game->duck, LOGICAL_WIDTH / 2.0f,
                LAKE_START_Y - duck_height);

    // Create object pools using factory
    create_entity_pools(game);

    // Initialize crabs using factory
    initialize_crabs(game);

    // Initialize jellyfish using factory
    initialize_jellyfish(game);
}

static void initialize_crabs(game_ptr game) {
    for (int i = 0; i < NUM_CRABS; i++) {
        size_t crab_index;
        crab_ptr crab =
            (crab_ptr)pool_acquire(&game->crab_pool, &crab_index);
        if (!crab) {
            break;  // Pool is full
        }

        // Use factory to create crab with random properties
        if (!create_crab(crab)) {
            pool_release(&game->crab_pool, crab_index);
        }
    }
}

static void initialize_jellyfish(game_ptr game) {
    // Calculate jellyfish positioning and movement parameters
    int jellyfish_zone_y = (int)(LOGICAL_HEIGHT * 0.7f);
    const int jellyfish_spacing = 1;

    // Random group movement parameters (all jellyfish move together)
    float speed = JELLYFISH_MIN_SPEED +
                  ((float)rand() / RAND_MAX) * JELLYFISH_SPEED_RANGE;
    bool moving_right = (rand() % 2) == 0;
    float group_velocity_x = moving_right ? speed : -speed;

    // Calculate starting position to center all jellyfish as a group
    float total_width = (JELLYFISH_WIDTH * NUM_JELLYFISH) +
                        (jellyfish_spacing * (NUM_JELLYFISH - 1));
    float start_x = (LOGICAL_WIDTH - total_width) / 2.0f;

    for (int i = 0; i < NUM_JELLYFISH; i++) {
        size_t jellyfish_index;
        jellyfish_ptr jellyfish = (jellyfish_ptr)pool_acquire(
            &game->jellyfish_pool, &jellyfish_index);
        if (!jellyfish) {
            break;  // Pool is full
        }

        // Calculate position for this jellyfish
        float x = start_x + (i * (JELLYFISH_WIDTH + jellyfish_spacing));
        float y = jellyfish_zone_y;

        // Use factory to create jellyfish
        create_jellyfish(jellyfish, x, y, group_velocity_x, moving_right, i);
    }
}

void cleanup_all_entities(game_ptr game) {
    // Use factory to destroy pools
    destroy_entity_pools(game);
}
