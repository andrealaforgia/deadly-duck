/**
 * @file collision_registry.h
 * @brief Collision strategy registry for dynamic handler lookup
 *
 * Provides registry pattern for collision strategy management.
 * Allows registering collision handlers by entity type pairs and
 * performing dynamic lookups during collision detection.
 */

#ifndef COLLISION_REGISTRY_H
#define COLLISION_REGISTRY_H

#include "collision_interface.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Maximum number of collision strategies that can be registered
 */
#define MAX_COLLISION_STRATEGIES 32

/**
 * @brief Entity type enumeration for collision registry
 */
typedef enum {
    ENTITY_TYPE_UNKNOWN = -1,
    ENTITY_TYPE_DUCK,
    ENTITY_TYPE_POPCORN,
    ENTITY_TYPE_CRAB,
    ENTITY_TYPE_JELLYFISH,
    ENTITY_TYPE_BRICK,
    ENTITY_TYPE_LANDED_BRICK,
    ENTITY_TYPE_DUCK_POSITION,  // Special type for position-based collision checks (uses point_t)
    ENTITY_TYPE_COUNT
} entity_type_t;

/**
 * @brief Collision strategy registry entry
 */
typedef struct {
    entity_type_t entity_a_type;
    entity_type_t entity_b_type;
    collision_strategy_t strategy;
    bool active;
} collision_registry_entry_t;

/**
 * @brief Collision strategy registry
 */
typedef struct {
    collision_registry_entry_t entries[MAX_COLLISION_STRATEGIES];
    size_t entry_count;
} collision_registry_t;

/**
 * @brief Initialize collision registry
 * @param registry Registry to initialize
 */
void collision_registry_init(collision_registry_t* registry);

/**
 * @brief Register a collision strategy for entity type pair
 * @param registry Collision registry
 * @param entity_a_type First entity type
 * @param entity_b_type Second entity type
 * @param strategy Collision strategy
 * @return true if registration successful, false otherwise
 */
bool collision_registry_register(collision_registry_t* registry,
                                 entity_type_t entity_a_type,
                                 entity_type_t entity_b_type,
                                 collision_strategy_t strategy);

/**
 * @brief Lookup collision strategy for entity type pair
 * @param registry Collision registry
 * @param entity_a_type First entity type
 * @param entity_b_type Second entity type
 * @return Collision strategy pointer, or NULL if not found
 */
collision_strategy_t* collision_registry_lookup(collision_registry_t* registry,
                                                entity_type_t entity_a_type,
                                                entity_type_t entity_b_type);

/**
 * @brief Get all registered collision strategies
 * @param registry Collision registry
 * @param strategies Output array for strategies
 * @param max_strategies Maximum number of strategies to return
 * @return Number of strategies returned
 */
size_t collision_registry_get_all(collision_registry_t* registry,
                                  collision_strategy_t** strategies,
                                  size_t max_strategies);

/**
 * @brief Clear all registered collision strategies
 * @param registry Collision registry to clear
 */
void collision_registry_clear(collision_registry_t* registry);

/**
 * @brief Get entity type from entity pointer
 * @param entity Entity pointer to inspect
 * @return Entity type, or ENTITY_TYPE_UNKNOWN if cannot determine
 */
entity_type_t get_entity_type_from_ptr(entity_ptr entity);

#endif // COLLISION_REGISTRY_H
