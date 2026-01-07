/**
 * @file entity_manager.h
 * @brief Centralized entity initialization and management
 *
 * Handles initialization of all game entities with their
 * starting positions, velocities, and states.
 */

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "game.h"

/**
 * @brief Initialize all game entities with object pools
 * @param game Game state containing entities to initialize
 */
void initialize_all_entities(game_ptr game);

/**
 * @brief Clean up all entity object pools
 * @param game Game state containing entity pools to destroy
 */
void cleanup_all_entities(game_ptr game);

#endif // ENTITY_MANAGER_H
