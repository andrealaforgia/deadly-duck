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
 * @brief Initialize all game entities
 * @param game Game state containing entities to initialize
 */
void initialize_all_entities(game_ptr game);

#endif // ENTITY_MANAGER_H
