/**
 * @file player_controller.c
 * @brief Player input controller implementation
 */

#include "player_controller.h"
#include "game.h"
#include "audio.h"
#include "projectile.h"
#include "duck.h"

bool player_handle_event(SDL_Event* event, game_ptr game) {
    switch (event->type) {
        case SDL_QUIT:
            return false;

        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }

            // Handle duck movement and shooting controls (only if duck is alive)
            if (!game->duck.dead) {
                if (event->key.keysym.sym == SDLK_LEFT) {
                    game->duck.vx = -DUCK_SPEED;
                    game->duck.facing_right = false;
                } else if (event->key.keysym.sym == SDLK_RIGHT) {
                    game->duck.vx = DUCK_SPEED;
                    game->duck.facing_right = true;
                } else if (event->key.keysym.sym == SDLK_SPACE) {
                    // Trigger shooting
                    game->duck.shooting = true;
                    game->duck.shoot_start_time = SDL_GetTicks();

                    // Play quack sound
                    play_sound(&game->audio_context, SOUND_QUACK);

                    // Spawn a popcorn projectile
                    const int duck_sprite_width = DUCK_WIDTH;
                    float offset = game->duck.facing_right ? duck_sprite_width * 0.7f : duck_sprite_width * 0.3f;

                    popcorn_spawn(game->popcorn, MAX_POPCORN,
                                 game->duck.x + offset,
                                 game->duck.y);
                }
            }
            break;

        case SDL_KEYUP:
            if (!game->duck.dead) {
                // Stop duck movement when key is released
                if (event->key.keysym.sym == SDLK_LEFT && game->duck.vx < 0) {
                    game->duck.vx = 0.0f;
                } else if (event->key.keysym.sym == SDLK_RIGHT && game->duck.vx > 0) {
                    game->duck.vx = 0.0f;
                }
            }
            break;
    }

    return true;
}

bool player_process_input(game_ptr game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (!player_handle_event(&event, game)) {
            return false;
        }
    }
    return true;
}
