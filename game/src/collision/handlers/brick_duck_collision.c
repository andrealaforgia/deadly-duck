/**
 * @file brick_duck_collision.c
 * @brief Brick-Duck collision strategy implementation
 */

#include "brick_duck_collision.h"
#include "collision.h"
#include "constants.h"
#include "clock.h"
#include "audio.h"
#include "sprite_atlas.h"
#include "event_system.h"
#include "game_events.h"
#include "brick.h"
#include "duck.h"

collision_result_t check_brick_duck_collision(entity_ptr a, entity_ptr b) {
    brick_ptr brick = (brick_ptr)a;
    duck_ptr duck = (duck_ptr)b;
    
    // Only check falling bricks against living duck
    if (!brick || !duck || !brick->active || brick->landed || duck->dead) {
        return no_collision_result();
    }
    
    // AABB collision detection
    bool collision = aabb_collision(duck->x, duck->y, DUCK_WIDTH, DUCK_HEIGHT,
                                   brick->x, brick->y, BRICK_WIDTH, BRICK_HEIGHT);
    
    if (collision) {
        collision_result_t result = {
            .collision_detected = true,
            .entity_a_destroyed = true,  // Brick will be destroyed
            .entity_b_destroyed = true,  // Duck will be killed
            .entity_a_modified = true,
            .entity_b_modified = true
        };
        return result;
    }
    
    return no_collision_result();
}

void respond_brick_duck_collision(game_ptr game, entity_ptr a, entity_ptr b, collision_result_t result) {
    if (!result.collision_detected) {
        return;
    }
    
    brick_ptr brick = (brick_ptr)a;
    (void)b; // Duck handling is done through game state
    
    // Kill duck (implement duck death logic)
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
    
    // Deactivate brick
    brick->active = false;
    // Note: Pool release is handled at a higher level
}

collision_strategy_t get_brick_duck_strategy(void) {
    collision_strategy_t strategy = {
        .check = check_brick_duck_collision,
        .respond = respond_brick_duck_collision,
        .name = "Brick-Duck Collision"
    };
    return strategy;
}
