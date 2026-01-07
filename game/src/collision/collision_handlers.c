/**
 * @file collision_handlers.c
 * @brief Simple collision response handlers implementation
 */

#include "collision_handlers.h"
#include "audio.h"
#include "brick.h"
#include "clock.h"
#include "collision_detection.h"
#include "constants.h"
#include "crab.h"
#include "duck.h"
#include "event_system.h"
#include "game_events.h"
#include "jellyfish.h"
#include "projectile.h"

bool handle_popcorn_crab_collision(game_ptr game, popcorn_ptr popcorn, crab_ptr crab) {
    if (!popcorn || !crab || !popcorn->active || popcorn->reflected || !crab->alive) {
        return false;
    }

    if (check_aabb_collision(popcorn->x, popcorn->y, POPCORN_WIDTH, POPCORN_HEIGHT, crab->x, crab->y, CRAB_WIDTH,
                             CRAB_HEIGHT)) {
        // Kill crab
        crab->alive = false;

        // Deactivate popcorn
        popcorn->active = false;

        // Play hit sound
        play_sound(&game->audio_context, SOUND_CRAB_HIT);

        // Publish collision event
        crab_destroyed_data_t event_data = {crab->x, crab->y};
        game_event_t event = {.type = GAME_EVENT_CRAB_DESTROYED, .data = &event_data, .data_size = sizeof(event_data)};
        publish(&game->event_system, &event);

        return true;
    }

    return false;
}

bool handle_popcorn_jellyfish_collision(game_ptr game, popcorn_ptr popcorn, jellyfish_ptr jellyfish) {
    (void)game; // Not used in this collision

    if (!popcorn || !jellyfish || !popcorn->active || popcorn->reflected) {
        return false;
    }

    if (check_aabb_collision(popcorn->x, popcorn->y, POPCORN_WIDTH, POPCORN_HEIGHT, jellyfish->x, jellyfish->y,
                             JELLYFISH_WIDTH, JELLYFISH_HEIGHT)) {
        // Reflect popcorn downward
        popcorn_reflect(popcorn);

        return true;
    }

    return false;
}

bool handle_popcorn_duck_collision(game_ptr game, popcorn_ptr popcorn, duck_ptr duck) {
    if (!popcorn || !duck || !popcorn->active || !popcorn->reflected || duck->dead) {
        return false;
    }

    if (check_aabb_collision(popcorn->x, popcorn->y, POPCORN_WIDTH, POPCORN_HEIGHT, duck->x, duck->y, DUCK_WIDTH,
                             DUCK_HEIGHT)) {
        // Kill duck
        duck->dead = true;
        duck->death_time = get_clock_ticks_ms();

        // Deactivate popcorn
        popcorn->active = false;

        // Play death sound
        play_sound(&game->audio_context, SOUND_DUCK_DEATH);

        // Publish death event
        duck_died_data_t event_data = {duck->x, duck->y};
        game_event_t event = {.type = GAME_EVENT_DUCK_DIED, .data = &event_data, .data_size = sizeof(event_data)};
        publish(&game->event_system, &event);

        return true;
    }

    return false;
}

bool handle_brick_duck_collision(game_ptr game, brick_ptr brick, duck_ptr duck) {
    if (!brick || !duck || !brick->active || brick->landed || duck->dead) {
        return false;
    }

    if (check_aabb_collision(duck->x, duck->y, DUCK_WIDTH, DUCK_HEIGHT, brick->x, brick->y, BRICK_WIDTH,
                             BRICK_HEIGHT)) {
        // Kill duck
        duck->dead = true;
        duck->death_time = get_clock_ticks_ms();

        // Play death sound
        play_sound(&game->audio_context, SOUND_DUCK_DEATH);

        // Publish death event
        duck_died_data_t event_data = {duck->x, duck->y};
        game_event_t event = {.type = GAME_EVENT_DUCK_DIED, .data = &event_data, .data_size = sizeof(event_data)};
        publish(&game->event_system, &event);

        return true;
    }

    return false;
}

bool check_duck_brick_landing_collision(game_ptr game, float duck_x) {
    if (!game) {
        return false;
    }

    // Check collision with all landed bricks
    for (size_t i = 0; i < game->brick_pool.capacity; i++) {
        if (pool_is_active(&game->brick_pool, i)) {
            brick_ptr brick = (brick_ptr)pool_get_at(&game->brick_pool, i);
            if (brick && brick->landed) {
                if (check_aabb_collision(duck_x, game->duck.y, DUCK_WIDTH, DUCK_HEIGHT, brick->x, brick->y, BRICK_WIDTH,
                                         BRICK_HEIGHT)) {
                    return true;
                }
            }
        }
    }

    return false;
}
