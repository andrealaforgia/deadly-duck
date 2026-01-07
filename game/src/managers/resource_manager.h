/**
 * @file resource_manager.h
 * @brief Coordinated game resource loading and cleanup
 *
 * Orchestrates loading and freeing of all game assets using focused
 * resource loading modules for textures, fonts, and audio.
 */

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdbool.h>
#include "game.h"

/**
 * @brief Load all game resources (graphics, audio, fonts)
 * @param game Game state to load resources into
 * @return true if all resources loaded successfully, false otherwise
 */
bool load_game_resources(game_ptr game);

/**
 * @brief Free all game resources
 * @param game Game state containing resources to free
 */
void free_game_resources(game_ptr game);

#endif // RESOURCE_MANAGER_H
