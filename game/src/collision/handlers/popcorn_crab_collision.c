/**
 * @file popcorn_crab_collision.c
 * @brief Popcorn-Crab collision strategy implementation
 */

#include "popcorn_crab_collision.h"
#include "collision.h"
#include "constants.h"
#include "audio.h"
#include "event_system.h"
#include "game_events.h"

collision_result_t check_popcorn_crab_collision(entity_ptr popcorn_ptr, entity_ptr crab_ptr) {
    popcorn_ptr popcorn = (popcorn_ptr)popcorn_ptr;
    crab_ptr crab = (crab_ptr)crab_ptr;
    
    if (!popcorn || !crab || !popcorn->active || popcorn->reflected || !crab->alive) {
        return no_collision_result();
    }
    
    // AABB collision detection
    bool collision = aabb_collision(popcorn->x, popcorn->y, POPCORN_WIDTH, POPCORN_HEIGHT,
                                   crab->x, crab->y, CRAB_WIDTH, CRAB_HEIGHT);
    
    if (collision) {
        collision_result_t result = {
            .collision_detected = true,
            .entity_a_destroyed = true,  // Popcorn will be destroyed
            .entity_b_destroyed = true,  // Crab will be killed
            .entity_a_modified = true,
            .entity_b_modified = true
        };
        return result;
    }
    
    return no_collision_result();
}

void respond_popcorn_crab_collision(game_ptr game, entity_ptr popcorn_ptr, entity_ptr crab_ptr, collision_result_t result) {
    if (!result.collision_detected) {
        return;
    }
    
    popcorn_ptr popcorn = (popcorn_ptr)popcorn_ptr;
    crab_ptr crab = (crab_ptr)crab_ptr;
    
    // Mark crab as dead
    crab->alive = false;
    
    // Deactivate popcorn
    popcorn->active = false;
    // Note: Pool release is handled at a higher level
    
    // Play crab hit sound
    play_sound(&game->audio_context, SOUND_CRAB_HIT);
    
    // Publish crab destroyed event (subscribers will award score)
    crab_destroyed_data_t event_data = {crab->x, crab->y};
    game_event_t event = {
        .type = GAME_EVENT_CRAB_DESTROYED,
        .data = &event_data,
        .data_size = sizeof(event_data)
    };
    publish(&game->event_system, &event);
}

collision_strategy_t get_popcorn_crab_strategy(void) {
    collision_strategy_t strategy = {
        .check = check_popcorn_crab_collision,
        .respond = respond_popcorn_crab_collision,
        .name = "Popcorn-Crab Collision"
    };
    return strategy;
}
