/**
 * @file collision_game_event_observer.h
 * @brief Advanced collision observer that publishes game events
 *
 * Provides an observer that monitors collision patterns and publishes
 * relevant game events like high collision rates, collision detection
 * events, and entity-specific collision counts.
 */

#ifndef COLLISION_GAME_EVENT_OBSERVER_H
#define COLLISION_GAME_EVENT_OBSERVER_H

#include "collision_observer.h"
#include "collision_registry.h"
#include "event_system.h"
#include "game_events.h"
#include "types.h"

/**
 * @brief Configuration for game event publishing thresholds
 */
typedef struct {
    size_t high_collision_rate_threshold;  // Collisions per second threshold
    size_t entity_collision_count_threshold;  // Per-entity collision count threshold
    bool publish_all_collisions;  // Whether to publish every collision detection
} collision_game_event_config_t;

/**
 * @brief Game event observer data structure
 */
typedef struct {
    event_system_ptr event_system;
    collision_game_event_config_t config;
    
    // Statistics tracking
    size_t total_collisions;
    size_t last_collision_rate;
    timestamp_ms_t last_rate_check_time;
    
    // Per-entity collision tracking
    size_t entity_collision_counts[ENTITY_TYPE_COUNT];
} collision_game_event_data_t;

/**
 * @brief Create game event publishing observer
 * @param event_system Game event system to publish to
 * @param config Configuration for event publishing
 * @param name Observer name
 * @return Initialized game event observer
 */
collision_observer_t* create_collision_game_event_observer(event_system_ptr event_system,
                                                          const collision_game_event_config_t* config,
                                                          const char* name);

/**
 * @brief Get default game event configuration
 * @return Default configuration struct
 */
collision_game_event_config_t get_default_game_event_config(void);

/**
 * @brief Get collision statistics from game event observer
 * @param observer Game event observer
 * @return Statistics data (read-only)
 */
const collision_game_event_data_t* get_game_event_observer_stats(collision_observer_t* observer);

/**
 * @brief Reset statistics in game event observer
 * @param observer Game event observer
 */
void reset_game_event_observer_stats(collision_observer_t* observer);

/**
 * @brief Destroy game event observer
 * @param observer Observer to destroy
 */
void destroy_collision_game_event_observer(collision_observer_t* observer);

#endif // COLLISION_GAME_EVENT_OBSERVER_H
