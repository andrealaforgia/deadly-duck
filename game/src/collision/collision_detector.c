/**
 * @file collision_detector.c
 * @brief Collision detection implementation
 */

#include "collision_detector.h"
#include "collision_strategy_factory.h"
#include "collision_interface.h"
#include "collision_template.h"
#include "collision_processors.h"
#include "projectile.h"
#include "brick.h"
#include "crab.h"
#include "duck.h"
#include "jellyfish.h"
#include "collision.h"
#include "geometry.h"

/**
 * @brief Enhanced collision processing using Template Method pattern
 * @param game Game state
 * @param entity_a_type First entity type
 * @param entity_b_type Second entity type 
 * @param pool_a First entity pool
 * @param pool_b Second entity pool
 */
static void process_entity_collisions_template(game_ptr game, entity_type_t entity_a_type, entity_type_t entity_b_type,
                                              object_pool_t* pool_a, object_pool_t* pool_b) {
    collision_registry_t* registry = get_collision_registry();
    if (!registry) return;
    
    collision_strategy_t* strategy = collision_registry_lookup(registry, entity_a_type, entity_b_type);
    if (!strategy) return;
    
    // Create pool-managed processor with Template Method pattern
    pool_collision_processor_t* processor = create_pool_processor(strategy, pool_a, pool_b, strategy->name);
    if (!processor) return;
    
    // Check all combinations of active entities using Template Method
    for (size_t i = 0; i < pool_a->capacity; i++) {
        if (!pool_is_active(pool_a, i)) continue;
        
        entity_ptr entity_a = pool_get_at(pool_a, i);
        if (!entity_a) continue;
        
        for (size_t j = 0; j < pool_b->capacity; j++) {
            if (!pool_is_active(pool_b, j)) continue;
            
            entity_ptr entity_b = pool_get_at(pool_b, j);
            if (!entity_b) continue;
            
            // Set pool indices for proper cleanup
            processor->index_a = i;
            processor->index_b = j;
            
            // Use Template Method pattern for collision processing
            processor->base.process_collision(&processor->base, game, entity_a, entity_b);
        }
    }
    
    destroy_collision_processor(&processor->base);
}

/**
 * @brief Legacy collision processing (for compatibility)
 */
static void process_entity_collisions(game_ptr game, entity_type_t entity_a_type, entity_type_t entity_b_type,
                                     object_pool_t* pool_a, object_pool_t* pool_b) {
    // Use enhanced template method by default
    process_entity_collisions_template(game, entity_a_type, entity_b_type, pool_a, pool_b);
}


bool check_duck_landed_brick_collision(game_ptr game, float new_duck_x) {
    collision_registry_t* registry = get_collision_registry();
    if (!registry) return false;
    
    collision_strategy_t* strategy = collision_registry_lookup(registry, ENTITY_TYPE_DUCK_POSITION, ENTITY_TYPE_LANDED_BRICK);
    if (!strategy) return false;
    
    // Create duck position for collision check
    point_t duck_pos = {new_duck_x, game->duck.y};
    
    // Check collision against all landed bricks
    for (size_t i = 0; i < game->brick_pool.capacity; i++) {
        if (!pool_is_active(&game->brick_pool, i)) continue;
        
        brick_ptr brick = (brick_ptr)pool_get_at(&game->brick_pool, i);
        if (!brick) continue;
        
        collision_result_t result = strategy->check((entity_ptr)&duck_pos, (entity_ptr)brick);
        if (result.collision_detected) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Enhanced duck collision processing using Template Method pattern
 * @param game Game state
 * @param entity_type Type of entities in the pool
 * @param pool Entity pool to check against duck
 */
static void process_duck_collisions_template(game_ptr game, entity_type_t entity_type, object_pool_t* pool) {
    if (game->duck.dead) return;
    
    collision_registry_t* registry = get_collision_registry();
    if (!registry) return;
    
    collision_strategy_t* strategy = collision_registry_lookup(registry, entity_type, ENTITY_TYPE_DUCK);
    if (!strategy) return;
    
    // Create filtered processor that checks for living duck
    filtered_collision_processor_t* processor = create_filtered_processor(strategy, living_duck_filter, strategy->name);
    if (!processor) return;
    
    // Check all active entities in pool against duck using Template Method
    for (size_t i = 0; i < pool->capacity; i++) {
        if (!pool_is_active(pool, i)) continue;
        
        entity_ptr entity = pool_get_at(pool, i);
        if (!entity) continue;
        
        // Use Template Method pattern for duck collision processing
        if (processor->base.process_collision(&processor->base, game, entity, (entity_ptr)&game->duck)) {
            // Handle pool cleanup if entity was destroyed
            collision_result_t result = strategy->check(entity, (entity_ptr)&game->duck);
            if (result.entity_a_destroyed) {
                pool_release(pool, i);
            }
            // Note: Duck destruction is handled by the strategy response
        }
    }
    
    destroy_collision_processor(&processor->base);
}

/**
 * @brief Legacy duck collision processing (for compatibility)
 */
static void process_duck_collisions(game_ptr game, entity_type_t entity_type, object_pool_t* pool) {
    // Use enhanced template method by default
    process_duck_collisions_template(game, entity_type, pool);
}

void process_all_collisions(game_ptr game) {
    // Process all entity-to-entity collisions using strategy pattern
    process_entity_collisions(game, ENTITY_TYPE_POPCORN, ENTITY_TYPE_JELLYFISH, 
                             &game->popcorn_pool, &game->jellyfish_pool);
    
    process_entity_collisions(game, ENTITY_TYPE_POPCORN, ENTITY_TYPE_CRAB,
                             &game->popcorn_pool, &game->crab_pool);
    
    // Process collisions with single duck entity
    process_duck_collisions(game, ENTITY_TYPE_POPCORN, &game->popcorn_pool);
    process_duck_collisions(game, ENTITY_TYPE_BRICK, &game->brick_pool);
}
