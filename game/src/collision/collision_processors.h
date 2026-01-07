/**
 * @file collision_processors.h
 * @brief Concrete collision processors using Template Method pattern
 *
 * Provides specialized collision processors for different entity types,
 * each implementing the Template Method pattern with type-specific logic.
 */

#ifndef COLLISION_PROCESSORS_H
#define COLLISION_PROCESSORS_H

#include "collision_template.h"
#include "projectile.h"
#include "crab.h"
#include "duck.h"
#include "brick.h"
#include "jellyfish.h"

/**
 * @brief Strategy-based collision processor
 * Uses the collision strategy pattern within the template method
 */
typedef struct {
    collision_processor_t base;
} strategy_collision_processor_t;

/**
 * @brief Pool-managed collision processor
 * Handles pool cleanup for destroyed entities
 */
typedef struct {
    collision_processor_t base;
    object_pool_t* pool_a;  // Pool containing entity_a
    object_pool_t* pool_b;  // Pool containing entity_b
    size_t index_a;         // Index of entity_a in pool_a
    size_t index_b;         // Index of entity_b in pool_b
} pool_collision_processor_t;

/**
 * @brief Filtered collision processor
 * Adds pre-processing filters for entity states
 */
typedef struct {
    collision_processor_t base;
    bool (*entity_filter)(entity_ptr entity);  // Filter function for entity validation
} filtered_collision_processor_t;

/**
 * @brief Create strategy-based collision processor
 * @param strategy Collision strategy to use
 * @param name Processor name
 * @return Initialized strategy processor
 */
strategy_collision_processor_t* create_strategy_processor(collision_strategy_t* strategy, const char* name);

/**
 * @brief Create pool-managed collision processor
 * @param strategy Collision strategy to use
 * @param pool_a Pool containing first entity type
 * @param pool_b Pool containing second entity type
 * @param name Processor name
 * @return Initialized pool processor
 */
pool_collision_processor_t* create_pool_processor(collision_strategy_t* strategy, 
                                                 object_pool_t* pool_a, 
                                                 object_pool_t* pool_b,
                                                 const char* name);

/**
 * @brief Create filtered collision processor
 * @param strategy Collision strategy to use
 * @param filter Entity validation filter
 * @param name Processor name
 * @return Initialized filtered processor
 */
filtered_collision_processor_t* create_filtered_processor(collision_strategy_t* strategy,
                                                         bool (*filter)(entity_ptr),
                                                         const char* name);

/**
 * @brief Destroy collision processor and free memory
 * @param processor Processor to destroy
 */
void destroy_collision_processor(collision_processor_t* processor);

/**
 * @brief Entity filters for common validation cases
 */
bool active_popcorn_filter(entity_ptr entity);
bool living_duck_filter(entity_ptr entity);
bool alive_crab_filter(entity_ptr entity);
bool falling_brick_filter(entity_ptr entity);
bool landed_brick_filter(entity_ptr entity);

#endif // COLLISION_PROCESSORS_H
