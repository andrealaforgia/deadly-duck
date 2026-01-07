/**
 * @file resource_manager.c
 * @brief Coordinated game resource loading and cleanup implementation
 */

#include "resource_manager.h"

#include "constants.h"
#include "graphics.h"
#include "texture_loader.h"
#include "audio_loader.h"
#include "font_loader.h"

bool load_game_resources(game_ptr game) {
    // Initialize graphics context using engine abstraction
    // Using display 0, display mode 0 (default), WINDOWED mode, with vsync
    game->graphics_context = init_graphics_context(0, 0, WINDOWED, true);

    // Set logical size to fixed dimensions from constants for pixel-perfect rendering
    set_logical_size(&game->graphics_context, LOGICAL_WIDTH, LOGICAL_HEIGHT);

    // Load textures using focused texture loader
    if (!load_game_textures(game, &game->graphics_context)) {
        return false;
    }

    // Load fonts using focused font loader
    if (!load_game_fonts(game, &game->graphics_context)) {
        return false;
    }

    // Load audio using focused audio loader
    if (!load_game_audio(game)) {
        return false;
    }

    return true;
}

void free_game_resources(game_ptr game) {
    // Free resources using focused loaders
    free_game_fonts(game);
    free_game_textures(game);
    free_game_audio(game);

    // Terminate graphics context (handles window, renderer, SDL cleanup)
    terminate_graphics_context(&game->graphics_context);
}
