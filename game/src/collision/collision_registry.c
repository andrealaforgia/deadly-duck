/**
 * @file collision_registry.c
 * @brief Collision strategy registry implementation
 */

#include "collision_registry.h"
#include <string.h>

void collision_registry_init(collision_registry_t* registry) {
    memset(registry, 0, sizeof(collision_registry_t));
    registry->entry_count = 0;
}

bool collision_registry_register(collision_registry_t* registry,
                                 entity_type_t entity_a_type,
                                 entity_type_t entity_b_type,
                                 collision_strategy_t strategy) {
    if (!registry || registry->entry_count >= MAX_COLLISION_STRATEGIES) {
        return false;
    }

    // Check if entry already exists (prevent duplicates)
    for (size_t i = 0; i < registry->entry_count; i++) {
        collision_registry_entry_t* entry = &registry->entries[i];
        if (entry->active &&
            ((entry->entity_a_type == entity_a_type && entry->entity_b_type == entity_b_type) ||
             (entry->entity_a_type == entity_b_type && entry->entity_b_type == entity_a_type))) {
            // Update existing entry
            entry->strategy = strategy;
            return true;
        }
    }

    // Add new entry
    collision_registry_entry_t* entry = &registry->entries[registry->entry_count];
    entry->entity_a_type = entity_a_type;
    entry->entity_b_type = entity_b_type;
    entry->strategy = strategy;
    entry->active = true;
    
    registry->entry_count++;
    return true;
}

collision_strategy_t* collision_registry_lookup(collision_registry_t* registry,
                                                entity_type_t entity_a_type,
                                                entity_type_t entity_b_type) {
    if (!registry) {
        return NULL;
    }

    for (size_t i = 0; i < registry->entry_count; i++) {
        collision_registry_entry_t* entry = &registry->entries[i];
        if (entry->active &&
            ((entry->entity_a_type == entity_a_type && entry->entity_b_type == entity_b_type) ||
             (entry->entity_a_type == entity_b_type && entry->entity_b_type == entity_a_type))) {
            return &entry->strategy;
        }
    }

    return NULL;
}

size_t collision_registry_get_all(collision_registry_t* registry,
                                  collision_strategy_t** strategies,
                                  size_t max_strategies) {
    if (!registry || !strategies) {
        return 0;
    }

    size_t count = 0;
    for (size_t i = 0; i < registry->entry_count && count < max_strategies; i++) {
        if (registry->entries[i].active) {
            strategies[count] = &registry->entries[i].strategy;
            count++;
        }
    }

    return count;
}

void collision_registry_clear(collision_registry_t* registry) {
    if (registry) {
        memset(registry, 0, sizeof(collision_registry_t));
        registry->entry_count = 0;
    }
}

entity_type_t get_entity_type_from_ptr(entity_ptr entity) {
    if (!entity) return ENTITY_TYPE_UNKNOWN;
    
    // Since entities don't have explicit entity_type fields, we need to determine
    // the type based on other characteristics or context. For now, return UNKNOWN
    // and let callers use strategy names or other context to determine type.
    
    // This function would need to be enhanced with better type detection
    // mechanisms if exact entity type identification is needed.
    
    return ENTITY_TYPE_UNKNOWN;
}
