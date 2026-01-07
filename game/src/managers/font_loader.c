/**
 * @file font_loader.c
 * @brief Font resource loading and management implementation
 */

#include "font_loader.h"

#include <stdio.h>

#include "bitmap_font.h"

bool load_game_fonts(game_ptr game, const graphics_context_ptr graphics_context) {
    // Load arcade font using engine's bitmap font system
    game->font = load_bitmap_font(graphics_context, "game/assets/sprites/arcade-font.png", 8, 7, 8, 32);
    if (!game->font.texture.texture) {
        printf("Failed to load arcade font\n");
        return false;
    }

    return true;
}

void free_game_fonts(game_ptr game) {
    // Free bitmap font
    free_bitmap_font(&game->font);
}
