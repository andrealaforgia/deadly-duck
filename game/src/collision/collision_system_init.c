/**
 * @file collision_system_init.c
 * @brief Complete collision system initialization and integration
 */

#include "collision_system_init.h"
#include "collision_strategy_factory.h"
#include "collision_observers.h"
#include "collision_event_bridge.h"
#include "collision_game_event_observer.h"

// Global state tracking
static bool system_initialized = false;
static collision_observer_t* g_game_event_observer = NULL;

bool initialize_collision_system(game_ptr game) {
    if (system_initialized) {
        return true; // Already initialized
    }
    
    if (!game) {
        return false;
    }
    
    // 1. Initialize collision strategy registry and factory
    collision_registry_t* registry = get_collision_registry();
    if (!registry) {
        return false; // Factory initialization failed
    }
    
    // 2. Setup default collision observers (stats, debug, performance)
    setup_default_collision_observers();
    
    // 3. Setup event system integration bridge
    setup_collision_event_integration(game);
    
    // 4. Create advanced game event observer for collision patterns
    collision_game_event_config_t config = get_default_game_event_config();
    config.publish_all_collisions = false; // Don't spam with every collision
    config.high_collision_rate_threshold = 30; // Lower threshold for more responsive events
    config.entity_collision_count_threshold = 5; // Track entity destruction patterns
    
    g_game_event_observer = create_collision_game_event_observer(&game->event_system, &config, "GameEvents");
    if (g_game_event_observer) {
        collision_subject_t* subject = get_collision_subject();
        collision_subject_subscribe(subject, COLLISION_EVENT_DETECTED, g_game_event_observer);
        collision_subject_subscribe(subject, COLLISION_EVENT_ENTITY_DESTROYED, g_game_event_observer);
    }
    
    system_initialized = true;
    return true;
}

void cleanup_collision_system(void) {
    if (!system_initialized) {
        return;
    }
    
    // Cleanup game event observer
    if (g_game_event_observer) {
        collision_subject_t* subject = get_collision_subject();
        collision_subject_unsubscribe(subject, COLLISION_EVENT_DETECTED, g_game_event_observer);
        collision_subject_unsubscribe(subject, COLLISION_EVENT_ENTITY_DESTROYED, g_game_event_observer);
        destroy_collision_game_event_observer(g_game_event_observer);
        g_game_event_observer = NULL;
    }
    
    // Cleanup event integration bridge
    cleanup_collision_event_integration();
    
    // Cleanup default observers
    cleanup_collision_observers();
    
    system_initialized = false;
}

bool is_collision_system_initialized(void) {
    return system_initialized;
}
