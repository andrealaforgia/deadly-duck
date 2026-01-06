/**
 * @file resource_manager.c
 * @brief Game resource loading and cleanup implementation
 */

#include "resource_manager.h"

#include <stdio.h>

#include "constants.h"
#include "graphics.h"
#include "texture.h"
#include "bitmap_font.h"
#include "audio.h"

bool load_game_resources(game_ptr game) {
    // Initialize graphics context using engine abstraction
    // Using display 0, display mode 0 (default), WINDOWED mode, with vsync
    game->graphics_context = init_graphics_context(0, 0, WINDOWED, true);

    // Set logical size to fixed dimensions from constants for pixel-perfect rendering
    set_logical_size(&game->graphics_context, LOGICAL_WIDTH, LOGICAL_HEIGHT);

    // Load sprite sheet using engine abstraction with white color key
    game->sprite_sheet = load_texture_with_colorkey(game->graphics_context.renderer, 
                                                   "game/assets/sprites/sprite_sheet_pixelart.png",
                                                   255, 255, 255);
    if (!game->sprite_sheet.texture) {
        printf("Failed to load sprite sheet\n");
        return false;
    }

    // Load cover image using engine's texture loader
    game->cover_image = load_texture(game->graphics_context.renderer, "game/assets/images/Deadly_Duck_Cover.jpg");
    if (!game->cover_image.texture) {
        printf("Failed to load cover image\n");
        return false;
    }

    // Store original cover dimensions for aspect ratio calculations
    game->cover_width = game->cover_image.width;
    game->cover_height = game->cover_image.height;

    // Load arcade font using engine's bitmap font system
    game->font = load_bitmap_font(&game->graphics_context, "game/assets/sprites/arcade-font.png", 8, 7, 8, 32);
    if (!game->font.texture.texture) {
        printf("Failed to load arcade font\n");
        return false;
    }

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

void free_game_resources(game_ptr game) {
    // Free bitmap font
    free_bitmap_font(&game->font);

    // Free textures
    free_texture(&game->cover_image);
    free_texture(&game->sprite_sheet);

    // Terminate audio context (handles all sound cleanup)
    terminate_audio_context(&game->audio_context);

    // Terminate graphics context (handles window, renderer, SDL cleanup)
    terminate_graphics_context(&game->graphics_context);
}
