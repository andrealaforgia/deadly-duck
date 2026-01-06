/**
 * @file resource_manager.h
 * @brief Game resource loading and cleanup
 *
 * Handles loading and freeing of all game assets including textures,
 * fonts, and audio resources.
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
