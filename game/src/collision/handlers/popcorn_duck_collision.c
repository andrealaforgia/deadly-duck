/**
 * @file popcorn_duck_collision.c
 * @brief Reflected Popcorn-Duck collision strategy implementation
 */

#include "popcorn_duck_collision.h"
#include "collision.h"
#include "constants.h"
#include "clock.h"
#include "audio.h"
#include "sprite_atlas.h"
#include "event_system.h"
#include "game_events.h"
#include "projectile.h"
#include "duck.h"

collision_result_t check_popcorn_duck_collision(entity_ptr a, entity_ptr b) {
    popcorn_ptr popcorn = (popcorn_ptr)a;
    duck_ptr duck = (duck_ptr)b;
    
    // Only check if duck is alive and popcorn is reflected
    if (!popcorn || !duck || !popcorn->active || !popcorn->reflected || duck->dead) {
        return no_collision_result();
    }
    
    // AABB collision detection
    bool collision = aabb_collision(popcorn->x, popcorn->y, POPCORN_WIDTH, POPCORN_HEIGHT,
                                   duck->x, duck->y, DUCK_WIDTH, DUCK_HEIGHT);
    
    if (collision) {
        collision_result_t result = {
            .collision_detected = true,
            .entity_a_destroyed = true,  // Popcorn will be destroyed
            .entity_b_destroyed = true,  // Duck will be killed
            .entity_a_modified = true,
            .entity_b_modified = true
        };
        return result;
    }
    
    return no_collision_result();
}

void respond_popcorn_duck_collision(game_ptr game, entity_ptr a, entity_ptr b, collision_result_t result) {
    if (!result.collision_detected) {
        return;
    }
    
    popcorn_ptr popcorn = (popcorn_ptr)a;
    (void)b; // Duck handling is done through game state
    
    // Kill duck (this function handles death logic, sound, events)
    // We'll need to add a kill_duck declaration or access it differently
    // For now, we'll implement the logic inline
    int current_time = get_clock_ticks_ms();
    game->duck.dead = true;
    game->duck.death_time = current_time;
    game->lives--;

    // Position dead duck to touch lake surface
    const int dead_duck_height = SPRITE_DUCK_DEAD.h * 2;  // Sprite height scaled 2x
    game->duck.y = LAKE_START_Y - dead_duck_height;

    // Play duck death sound
    play_sound(&game->audio_context, SOUND_DUCK_DEATH);

    // Publish duck died event
    duck_died_data_t event_data = {game->duck.x, game->duck.y};
    game_event_t event = {
        .type = GAME_EVENT_DUCK_DIED,
        .data = &event_data,
        .data_size = sizeof(event_data)
    };
    publish(&game->event_system, &event);
    
    // Deactivate popcorn
    popcorn->active = false;
    // Note: Pool release is handled at a higher level
}

collision_strategy_t get_popcorn_duck_strategy(void) {
    collision_strategy_t strategy = {
        .check = check_popcorn_duck_collision,
        .respond = respond_popcorn_duck_collision,
        .name = "Reflected Popcorn-Duck Collision"
    };
    return strategy;
}
