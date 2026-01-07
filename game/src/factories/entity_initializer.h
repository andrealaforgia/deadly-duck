/**
 * @file entity_initializer.h
 * @brief High-level entity initialization using factory pattern
 *
 * Provides initialization functions that orchestrate entity creation
 * using the entity factory. Separates creation logic from initialization
 * workflow.
 */

#ifndef ENTITY_INITIALIZER_H
#define ENTITY_INITIALIZER_H

#include "game.h"

/**
 * @brief Initialize all game entities using entity factory
 * @param game Game state containing entities to initialize
 */
void initialize_all_entities(game_ptr game);

/**
 * @brief Clean up all entity resources
 * @param game Game state containing entities to clean up
 */
void cleanup_all_entities(game_ptr game);

#endif // ENTITY_INITIALIZER_H
