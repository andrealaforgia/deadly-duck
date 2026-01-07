/**
 * @file collision_system_init.h
 * @brief Complete collision system initialization and integration
 *
 * Provides unified initialization for the entire collision system including
 * strategy registry, observer pattern, template methods, and event integration.
 */

#ifndef COLLISION_SYSTEM_INIT_H
#define COLLISION_SYSTEM_INIT_H

#include "game.h"
#include <stdbool.h>

/**
 * @brief Initialize the complete collision system
 * @param game Game instance to initialize collision system for
 * @return true if initialization successful, false otherwise
 * 
 * This function sets up:
 * 1. Collision strategy registry and factory
 * 2. Observer pattern infrastructure
 * 3. Default collision observers (stats, debug, performance)
 * 4. Event system integration bridge
 * 5. Advanced game event observers
 */
bool initialize_collision_system(game_ptr game);

/**
 * @brief Cleanup the complete collision system
 * 
 * This function cleans up:
 * 1. All collision observers
 * 2. Event system integration
 * 3. Registry and factory resources
 */
void cleanup_collision_system(void);

/**
 * @brief Check if collision system is initialized
 * @return true if collision system is ready, false otherwise
 */
bool is_collision_system_initialized(void);

#endif // COLLISION_SYSTEM_INIT_H
