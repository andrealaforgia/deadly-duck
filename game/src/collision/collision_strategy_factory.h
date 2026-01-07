/**
 * @file collision_strategy_factory.h
 * @brief Factory for creating and registering collision strategies
 *
 * Provides centralized factory for collision strategy creation and registration.
 * Initializes the collision registry with all available collision handlers.
 */

#ifndef COLLISION_STRATEGY_FACTORY_H
#define COLLISION_STRATEGY_FACTORY_H

#include "collision_registry.h"

/**
 * @brief Initialize collision registry with all available strategies
 * @param registry Collision registry to populate
 * @return true if initialization successful, false otherwise
 */
bool collision_strategy_factory_init(collision_registry_t* registry);

/**
 * @brief Create and return global collision registry instance
 * @return Pointer to initialized collision registry
 */
collision_registry_t* get_collision_registry(void);

#endif // COLLISION_STRATEGY_FACTORY_H
