/**
 * @file entity_factory.h
 * @brief Entity creation factory following Factory pattern
 *
 * Provides focused entity creation functions without lifecycle management.
 * Each function creates and configures specific entity types with proper
 * initialization parameters.
 */

#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include "brick.h"
#include "crab.h"
#include "duck.h"
#include "game.h"
#include "jellyfish.h"
#include "projectile.h"

/**
 * @brief Create and initialize a duck entity at specified position
 * @param duck Duck entity to initialize
 * @param x Initial X position
 * @param y Initial Y position
 */
void create_duck(duck_ptr duck, float x, float y);

/**
 * @brief Create and initialize a crab entity with random properties
 * @param crab Crab entity to initialize
 * @return true if creation successful, false otherwise
 */
bool create_crab(crab_ptr crab);

/**
 * @brief Create and initialize a jellyfish entity
 * @param jellyfish Jellyfish entity to initialize
 * @param x Initial X position
 * @param y Initial Y position
 * @param group_velocity_x Velocity for group movement
 * @param moving_right Direction of movement
 * @param anim_offset Animation frame offset
 */
void create_jellyfish(jellyfish_ptr jellyfish, float x, float y, float group_velocity_x, bool moving_right,
                      int anim_offset);

/**
 * @brief Create and initialize object pools for all entity types
 * @param game Game state to initialize pools for
 */
void create_entity_pools(game_ptr game);

/**
 * @brief Destroy all entity object pools
 * @param game Game state containing pools to destroy
 */
void destroy_entity_pools(game_ptr game);

#endif // ENTITY_FACTORY_H
