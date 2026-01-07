/**
 * @file collision_interface.h
 * @brief Collision detection interface and strategy pattern definitions
 *
 * Provides abstract interface for collision detection between entities.
 * Supports pluggable collision strategies using function pointers to
 * eliminate hardcoded collision logic in the main detector.
 */

#ifndef COLLISION_INTERFACE_H
#define COLLISION_INTERFACE_H

#include <stdbool.h>
#include "game.h"

/**
 * @brief Forward declarations for entity types
 */
typedef void* entity_ptr;

/**
 * @brief Collision result data
 */
typedef struct {
    bool collision_detected;
    bool entity_a_destroyed;
    bool entity_b_destroyed;
    bool entity_a_modified;
    bool entity_b_modified;
} collision_result_t;

/**
 * @brief Collision detection function pointer type
 * @param a First entity pointer
 * @param b Second entity pointer
 * @return Collision result indicating what happened
 */
typedef collision_result_t (*collision_check_fn)(entity_ptr a, entity_ptr b);

/**
 * @brief Collision response function pointer type
 * @param game Game state for accessing context
 * @param a First entity pointer
 * @param b Second entity pointer
 * @param result Collision result from detection phase
 */
typedef void (*collision_response_fn)(game_ptr game, entity_ptr a, entity_ptr b, collision_result_t result);

/**
 * @brief Complete collision strategy combining detection and response
 */
typedef struct {
    collision_check_fn check;
    collision_response_fn respond;
    const char* name;  // For debugging
} collision_strategy_t;

/**
 * @brief Generic collision detection interface
 * @param a First entity
 * @param b Second entity
 * @return true if collision detected, false otherwise
 */
bool check_collision(entity_ptr a, entity_ptr b);

/**
 * @brief Apply collision strategy between two entities
 * @param game Game state
 * @param strategy Collision strategy to apply
 * @param a First entity
 * @param b Second entity
 * @return true if collision occurred and was handled
 */
bool apply_collision_strategy(game_ptr game, collision_strategy_t* strategy, entity_ptr a, entity_ptr b);

/**
 * @brief Create a default collision result (no collision)
 * @return Collision result with no collision detected
 */
collision_result_t no_collision_result(void);

/**
 * @brief Create a collision result for simple collision detection
 * @param collision_occurred true if collision happened
 * @return Basic collision result
 */
collision_result_t simple_collision_result(bool collision_occurred);

#endif // COLLISION_INTERFACE_H
