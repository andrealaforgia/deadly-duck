/**
 * @file audio_loader.c
 * @brief Audio resource loading and management implementation
 */

#include "audio_loader.h"

#include <stdio.h>

#include "audio.h"
#include "constants.h"

bool load_game_audio(game_ptr game) {
    // Initialize audio context using engine's audio system
    game->audio_context = init_audio_context(NUM_SOUNDS, MIX_MAX_VOLUME);

    // Load all sound effects
    if (!load_sound(&game->audio_context, SOUND_QUACK, "game/assets/sounds/", "quack.mp3")) {
        printf("Failed to load quack sound\n");
        return false;
    }

    if (!load_sound(&game->audio_context, SOUND_CRAB_HIT, "game/assets/sounds/", "crab_hit.mp3")) {
        printf("Failed to load crab hit sound\n");
        return false;
    }

    if (!load_sound(&game->audio_context, SOUND_BRICK_DROP, "game/assets/sounds/", "brick_drop.mp3")) {
        printf("Failed to load brick drop sound\n");
        return false;
    }

    if (!load_sound(&game->audio_context, SOUND_DUCK_DEATH, "game/assets/sounds/", "duck_death.mp3")) {
        printf("Failed to load duck death sound\n");
        return false;
    }

    return true;
}

void free_game_audio(game_ptr game) {
    // Terminate audio context (handles all sound cleanup)
    terminate_audio_context(&game->audio_context);
}
