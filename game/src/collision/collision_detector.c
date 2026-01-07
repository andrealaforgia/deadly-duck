/**
 * @file collision_detector.c
 * @brief Collision detection implementation
 */

#include "collision_detector.h"
#include "audio.h"
#include "projectile.h"
#include "brick.h"
#include "crab.h"
#include "duck.h"
#include "jellyfish.h"
#include "sprite_atlas.h"
#include "constants.h"
#include "score.h"
#include "clock.h"
#include "event_system.h"
#include "game_events.h"
#include "collision.h"

static void kill_duck(game_ptr game) {
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
}

static void check_popcorn_jellyfish_collisions(game_ptr game) {
    for (size_t i = 0; i < game->popcorn_pool.capacity; i++) {
        if (!pool_is_active(&game->popcorn_pool, i)) continue;
        
        popcorn_ptr popcorn = (popcorn_ptr)pool_get_at(&game->popcorn_pool, i);
        if (!popcorn || !popcorn->active || popcorn->reflected) continue;

        for (size_t j = 0; j < game->jellyfish_pool.capacity; j++) {
            if (!pool_is_active(&game->jellyfish_pool, j)) continue;
            
            jellyfish_ptr jellyfish = (jellyfish_ptr)pool_get_at(&game->jellyfish_pool, j);
            if (!jellyfish) continue;
            
            // AABB collision detection with jellyfish
            bool collision = aabb_collision(popcorn->x, popcorn->y, POPCORN_WIDTH, POPCORN_HEIGHT,
                                          jellyfish->x, jellyfish->y, JELLYFISH_WIDTH, JELLYFISH_HEIGHT);

            if (collision) {
                // Reflect popcorn downward
                popcorn_reflect(popcorn);
                break;  // Popcorn can only be reflected once
            }
        }
    }
}

static void check_popcorn_crab_collisions(game_ptr game) {
    for (size_t i = 0; i < game->popcorn_pool.capacity; i++) {
        if (!pool_is_active(&game->popcorn_pool, i)) continue;
        
        popcorn_ptr popcorn = (popcorn_ptr)pool_get_at(&game->popcorn_pool, i);
        if (!popcorn || !popcorn->active || popcorn->reflected) continue;

        for (size_t j = 0; j < game->crab_pool.capacity; j++) {
            if (!pool_is_active(&game->crab_pool, j)) continue;
            
            crab_ptr crab = (crab_ptr)pool_get_at(&game->crab_pool, j);
            if (!crab || !crab->alive) continue;

            // AABB collision detection
            bool collision = aabb_collision(popcorn->x, popcorn->y, POPCORN_WIDTH, POPCORN_HEIGHT,
                                          crab->x, crab->y, CRAB_WIDTH, CRAB_HEIGHT);

            if (collision) {
                // Mark crab as dead
                crab->alive = false;

                // Deactivate popcorn
                popcorn->active = false;
                pool_release(&game->popcorn_pool, i);

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

                break;  // Popcorn can only hit one crab
            }
        }
    }
}

static void check_reflected_popcorn_duck_collisions(game_ptr game) {
    if (game->duck.dead) return;

    for (size_t i = 0; i < game->popcorn_pool.capacity; i++) {
        if (!pool_is_active(&game->popcorn_pool, i)) continue;
        
        popcorn_ptr popcorn = (popcorn_ptr)pool_get_at(&game->popcorn_pool, i);
        if (!popcorn || !popcorn->active || !popcorn->reflected) continue;

        // AABB collision detection with duck
        bool collision = aabb_collision(popcorn->x, popcorn->y, POPCORN_WIDTH, POPCORN_HEIGHT,
                                      game->duck.x, game->duck.y, DUCK_WIDTH, DUCK_HEIGHT);

        if (collision) {
            kill_duck(game);

            // Deactivate popcorn
            popcorn->active = false;
            pool_release(&game->popcorn_pool, i);

            break;  // Only one popcorn can hit the duck
        }
    }
}

static void check_brick_duck_collisions(game_ptr game) {
    if (game->duck.dead) return;

    for (size_t i = 0; i < game->brick_pool.capacity; i++) {
        if (!pool_is_active(&game->brick_pool, i)) continue;
        
        brick_ptr brick = (brick_ptr)pool_get_at(&game->brick_pool, i);
        if (!brick || !brick->active || brick->landed) continue;  // Only check falling bricks

        // AABB collision detection
        bool collision = aabb_collision(game->duck.x, game->duck.y, DUCK_WIDTH, DUCK_HEIGHT,
                                      brick->x, brick->y, BRICK_WIDTH, BRICK_HEIGHT);

        if (collision) {
            kill_duck(game);

            // Deactivate the brick
            brick->active = false;
            pool_release(&game->brick_pool, i);

            break;  // Only one brick can hit the duck
        }
    }
}

bool check_duck_landed_brick_collision(game_ptr game, float new_duck_x) {
    for (size_t i = 0; i < game->brick_pool.capacity; i++) {
        if (!pool_is_active(&game->brick_pool, i)) continue;
        
        brick_ptr brick = (brick_ptr)pool_get_at(&game->brick_pool, i);
        if (!brick || !brick->landed) continue;
        
        // Check if duck would collide with brick
        bool collision = aabb_collision(new_duck_x, game->duck.y, DUCK_WIDTH, DUCK_HEIGHT,
                                      brick->x, brick->y, BRICK_WIDTH, BRICK_HEIGHT);

        if (collision) {
            return true;
        }
    }
    return false;
}

void process_all_collisions(game_ptr game) {
    check_popcorn_jellyfish_collisions(game);
    check_popcorn_crab_collisions(game);
    check_reflected_popcorn_duck_collisions(game);
    check_brick_duck_collisions(game);
}
