/**
 * @file texture_loader.c
 * @brief Texture resource loading and management implementation
 */

#include "texture_loader.h"

#include <stdio.h>

#include "constants.h"
#include "texture.h"

bool load_game_textures(game_ptr game, const graphics_context_ptr graphics_context) {
    // Load sprite sheet using engine abstraction with white color key
    game->sprite_sheet = load_texture_with_colorkey(graphics_context->renderer,
                                                    "game/assets/sprites/sprite_sheet_pixelart.png", 255, 255, 255);
    if (!game->sprite_sheet.texture) {
        printf("Failed to load sprite sheet\n");
        return false;
    }

    // Load cover image using engine's texture loader
    game->cover_image = load_texture(graphics_context->renderer, "game/assets/images/Deadly_Duck_Cover.jpg");
    if (!game->cover_image.texture) {
        printf("Failed to load cover image\n");
        return false;
    }

    // Store original cover dimensions for aspect ratio calculations
    game->cover_width = game->cover_image.width;
    game->cover_height = game->cover_image.height;

    return true;
}

void free_game_textures(game_ptr game) {
    // Free textures
    free_texture(&game->cover_image);
    free_texture(&game->sprite_sheet);
}
