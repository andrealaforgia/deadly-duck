/**
 * @file collision_template.h
 * @brief Template Method pattern for collision processing
 *
 * Provides a template algorithm for collision processing with customizable steps.
 * Defines the skeleton of collision detection while allowing subclasses to override
 * specific steps of the algorithm.
 */

#ifndef COLLISION_TEMPLATE_H
#define COLLISION_TEMPLATE_H

#include "collision_interface.h"
#include "object_pool.h"
#include <stdbool.h>

/**
 * @brief Abstract collision processor using Template Method pattern
 */
typedef struct collision_processor_t {
    /**
     * @brief Template method defining collision processing algorithm
     * @param self Processor instance
     * @param game Game state
     * @param entity_a First entity
     * @param entity_b Second entity
     * @return true if collision was processed
     */
    bool (*process_collision)(struct collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b);
    
    /**
     * @brief Pre-processing hook (e.g., validation, filtering)
     * @param self Processor instance
     * @param entity_a First entity
     * @param entity_b Second entity
     * @return true if processing should continue
     */
    bool (*pre_process)(struct collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b);
    
    /**
     * @brief Execute collision detection
     * @param self Processor instance
     * @param entity_a First entity
     * @param entity_b Second entity
     * @return collision result
     */
    collision_result_t (*detect_collision)(struct collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b);
    
    /**
     * @brief Execute collision response
     * @param self Processor instance
     * @param game Game state
     * @param entity_a First entity
     * @param entity_b Second entity
     * @param result Collision detection result
     */
    void (*respond_collision)(struct collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result);
    
    /**
     * @brief Post-processing hook (e.g., cleanup, logging)
     * @param self Processor instance
     * @param game Game state
     * @param entity_a First entity
     * @param entity_b Second entity
     * @param result Collision result
     */
    void (*post_process)(struct collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result);
    
    /**
     * @brief Strategy used by this processor
     */
    collision_strategy_t* strategy;
    
    /**
     * @brief Processor name for debugging
     */
    const char* name;
} collision_processor_t;

/**
 * @brief Initialize base collision processor
 * @param processor Processor to initialize
 * @param strategy Collision strategy to use
 * @param name Processor name
 */
void collision_processor_init(collision_processor_t* processor, collision_strategy_t* strategy, const char* name);

/**
 * @brief Template method implementation - defines the collision processing algorithm
 * @param self Processor instance
 * @param game Game state
 * @param entity_a First entity
 * @param entity_b Second entity
 * @return true if collision was processed successfully
 */
bool collision_processor_template_method(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b);

/**
 * @brief Default pre-processing implementation
 * @param self Processor instance
 * @param entity_a First entity
 * @param entity_b Second entity
 * @return true if processing should continue
 */
bool collision_processor_default_pre_process(collision_processor_t* self, entity_ptr entity_a, entity_ptr entity_b);

/**
 * @brief Default post-processing implementation (no-op)
 * @param self Processor instance
 * @param game Game state
 * @param entity_a First entity
 * @param entity_b Second entity
 * @param result Collision result
 */
void collision_processor_default_post_process(collision_processor_t* self, game_ptr game, entity_ptr entity_a, entity_ptr entity_b, collision_result_t result);

#endif // COLLISION_TEMPLATE_H
