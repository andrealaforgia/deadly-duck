/**
 * @file resource_manager.c
 * @brief Game resource loading and cleanup implementation
 */

#include "resource_manager.h"

#include <stdio.h>
#include <SDL_image.h>

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
    SDL_RenderSetLogicalSize(game->graphics_context.renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);

    // Load sprite sheet with white color key (special handling needed)
    // The engine's load_texture uses black as transparent, but this sprite sheet uses white
    SDL_Surface* sprite_surface = IMG_Load("game/assets/sprites/sprite_sheet_pixelart.png");
    if (!sprite_surface) {
        printf("Failed to load sprite sheet surface\n");
        return false;
    }

    // Set white as transparent color key
    SDL_SetColorKey(sprite_surface, SDL_TRUE, SDL_MapRGB(sprite_surface->format, 255, 255, 255));

    // Create texture from surface
    game->sprite_sheet.texture = SDL_CreateTextureFromSurface(game->graphics_context.renderer, sprite_surface);
    game->sprite_sheet.width = sprite_surface->w;
    game->sprite_sheet.height = sprite_surface->h;
    SDL_FreeSurface(sprite_surface);

    if (!game->sprite_sheet.texture) {
        printf("Failed to create sprite sheet texture\n");
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
