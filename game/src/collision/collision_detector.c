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
#include "event_system.h"
#include "game_events.h"

static void kill_duck(game_ptr game) {
    Uint32 current_time = SDL_GetTicks();
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

void check_popcorn_jellyfish_collisions(game_ptr game) {
    for (int i = 0; i < MAX_POPCORN; i++) {
        if (!game->popcorn[i].active || game->popcorn[i].reflected) continue;

        for (int j = 0; j < NUM_JELLYFISH; j++) {
            // AABB collision detection with jellyfish
            bool collision =
                game->popcorn[i].x < game->jellyfish[j].x + (16 * 2) &&  // Jellyfish width
                game->popcorn[i].x + POPCORN_WIDTH > game->jellyfish[j].x &&
                game->popcorn[i].y < game->jellyfish[j].y + (13 * 2) &&  // Jellyfish height
                game->popcorn[i].y + POPCORN_HEIGHT > game->jellyfish[j].y;

            if (collision) {
                // Reflect popcorn downward
                popcorn_reflect(&game->popcorn[i]);
                break;  // Popcorn can only be reflected once
            }
        }
    }
}

void check_popcorn_crab_collisions(game_ptr game) {
    for (int i = 0; i < MAX_POPCORN; i++) {
        if (!game->popcorn[i].active || game->popcorn[i].reflected) continue;

        for (int j = 0; j < NUM_CRABS; j++) {
            if (!game->crabs[j].alive) continue;

            // AABB collision detection
            bool collision =
                game->popcorn[i].x < game->crabs[j].x + CRAB_WIDTH &&
                game->popcorn[i].x + POPCORN_WIDTH > game->crabs[j].x &&
                game->popcorn[i].y < game->crabs[j].y + CRAB_HEIGHT &&
                game->popcorn[i].y + POPCORN_HEIGHT > game->crabs[j].y;

            if (collision) {
                // Mark crab as dead
                game->crabs[j].alive = false;

                // Deactivate popcorn
                game->popcorn[i].active = false;

                // Play crab hit sound
                play_sound(&game->audio_context, SOUND_CRAB_HIT);

                // Publish crab destroyed event (subscribers will award score)
                crab_destroyed_data_t event_data = {game->crabs[j].x, game->crabs[j].y};
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

void check_reflected_popcorn_duck_collisions(game_ptr game) {
    if (game->duck.dead) return;

    for (int i = 0; i < MAX_POPCORN; i++) {
        if (!game->popcorn[i].active || !game->popcorn[i].reflected) continue;

        // AABB collision detection with duck
        bool collision =
            game->popcorn[i].x < game->duck.x + DUCK_WIDTH &&
            game->popcorn[i].x + POPCORN_WIDTH > game->duck.x &&
            game->popcorn[i].y < game->duck.y + DUCK_HEIGHT &&
            game->popcorn[i].y + POPCORN_HEIGHT > game->duck.y;

        if (collision) {
            kill_duck(game);

            // Deactivate popcorn
            game->popcorn[i].active = false;

            break;  // Only one popcorn can hit the duck
        }
    }
}

void check_brick_duck_collisions(game_ptr game) {
    if (game->duck.dead) return;

    for (int i = 0; i < MAX_BRICKS; i++) {
        if (!game->bricks[i].active || game->bricks[i].landed) continue;  // Only check falling bricks

        // AABB collision detection
        bool collision =
            game->duck.x < game->bricks[i].x + BRICK_WIDTH &&
            game->duck.x + DUCK_WIDTH > game->bricks[i].x &&
            game->duck.y < game->bricks[i].y + BRICK_HEIGHT &&
            game->duck.y + DUCK_HEIGHT > game->bricks[i].y;

        if (collision) {
            kill_duck(game);

            // Deactivate the brick
            game->bricks[i].active = false;

            break;  // Only one brick can hit the duck
        }
    }
}

bool check_duck_landed_brick_collision(game_ptr game, float new_duck_x) {
    for (int i = 0; i < MAX_BRICKS; i++) {
        if (game->bricks[i].landed) {
            // Check if duck would collide with brick
            bool collision =
                new_duck_x < game->bricks[i].x + BRICK_WIDTH &&
                new_duck_x + DUCK_WIDTH > game->bricks[i].x &&
                game->duck.y < game->bricks[i].y + BRICK_HEIGHT &&
                game->duck.y + DUCK_HEIGHT > game->bricks[i].y;

            if (collision) {
                return true;
            }
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
