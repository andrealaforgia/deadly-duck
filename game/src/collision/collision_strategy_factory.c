/**
 * @file collision_strategy_factory.c
 * @brief Factory for creating and registering collision strategies implementation
 */

#include "collision_strategy_factory.h"
#include "handlers/popcorn_jellyfish_collision.h"
#include "handlers/popcorn_crab_collision.h"
#include "handlers/popcorn_duck_collision.h"
#include "handlers/brick_duck_collision.h"
#include "handlers/duck_landed_brick_collision.h"

// Global collision registry instance
static collision_registry_t global_collision_registry;
static bool registry_initialized = false;

bool collision_strategy_factory_init(collision_registry_t* registry) {
    if (!registry) {
        return false;
    }

    // Initialize the registry
    collision_registry_init(registry);

    // Register all collision strategies
    
    // Popcorn-Jellyfish collision (reflection)
    collision_strategy_t popcorn_jellyfish_strategy = get_popcorn_jellyfish_strategy();
    if (!collision_registry_register(registry, ENTITY_TYPE_POPCORN, ENTITY_TYPE_JELLYFISH, popcorn_jellyfish_strategy)) {
        return false;
    }

    // Popcorn-Crab collision (destruction + scoring)
    collision_strategy_t popcorn_crab_strategy = get_popcorn_crab_strategy();
    if (!collision_registry_register(registry, ENTITY_TYPE_POPCORN, ENTITY_TYPE_CRAB, popcorn_crab_strategy)) {
        return false;
    }

    // Reflected Popcorn-Duck collision (duck death)
    collision_strategy_t popcorn_duck_strategy = get_popcorn_duck_strategy();
    if (!collision_registry_register(registry, ENTITY_TYPE_POPCORN, ENTITY_TYPE_DUCK, popcorn_duck_strategy)) {
        return false;
    }

    // Falling Brick-Duck collision (duck death)
    collision_strategy_t brick_duck_strategy = get_brick_duck_strategy();
    if (!collision_registry_register(registry, ENTITY_TYPE_BRICK, ENTITY_TYPE_DUCK, brick_duck_strategy)) {
        return false;
    }

    // Duck-Landed Brick collision (movement blocking)
    collision_strategy_t duck_brick_strategy = get_duck_position_brick_strategy();
    if (!collision_registry_register(registry, ENTITY_TYPE_DUCK_POSITION, ENTITY_TYPE_LANDED_BRICK, duck_brick_strategy)) {
        return false;
    }

    return true;
}

collision_registry_t* get_collision_registry(void) {
    if (!registry_initialized) {
        if (collision_strategy_factory_init(&global_collision_registry)) {
            registry_initialized = true;
        }
    }
    return registry_initialized ? &global_collision_registry : NULL;
}
