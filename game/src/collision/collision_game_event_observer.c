/**
 * @file collision_game_event_observer.c
 * @brief Advanced collision observer that publishes game events
 */

#include "collision_game_event_observer.h"
#include "collision_registry.h"
#include "clock.h"
#include <stdlib.h>
#include <string.h>

// Forward declarations
static void game_event_observer_callback(collision_observer_t* observer, const collision_event_data_t* event);
static void publish_collision_detected_event(collision_game_event_data_t* data, const collision_event_data_t* collision_event);
static void check_and_publish_high_collision_rate(collision_game_event_data_t* data);
static void check_and_publish_entity_collision_count(collision_game_event_data_t* data, entity_type_t entity_type);

collision_observer_t* create_collision_game_event_observer(event_system_ptr event_system,
                                                          const collision_game_event_config_t* config,
                                                          const char* name) {
    collision_observer_t* observer = malloc(sizeof(collision_observer_t));
    if (!observer) return NULL;
    
    collision_game_event_data_t* data = malloc(sizeof(collision_game_event_data_t));
    if (!data) {
        free(observer);
        return NULL;
    }
    
    // Initialize data
    data->event_system = event_system;
    data->config = config ? *config : get_default_game_event_config();
    data->total_collisions = 0;
    data->last_collision_rate = 0;
    data->last_rate_check_time = get_clock_ticks_ms();
    
    // Initialize entity collision counts
    for (int i = 0; i < ENTITY_TYPE_COUNT; i++) {
        data->entity_collision_counts[i] = 0;
    }
    
    // Setup observer
    observer->on_collision_event = game_event_observer_callback;
    observer->observer_name = name;
    observer->user_data = data;
    observer->active = true;
    
    return observer;
}

static void game_event_observer_callback(collision_observer_t* observer, const collision_event_data_t* event) {
    collision_game_event_data_t* data = (collision_game_event_data_t*)observer->user_data;
    if (!data || !data->event_system) return;
    
    switch (event->type) {
        case COLLISION_EVENT_DETECTED:
            data->total_collisions++;
            
            // Publish collision detected event if configured
            if (data->config.publish_all_collisions) {
                publish_collision_detected_event(data, event);
            }
            
            // Check for high collision rate
            check_and_publish_high_collision_rate(data);
            break;
            
        case COLLISION_EVENT_ENTITY_DESTROYED:
            // Track entity collision counts
            if (event->result.entity_a_destroyed) {
                entity_type_t type_a = get_entity_type_from_ptr(event->entity_a);
                if (type_a != ENTITY_TYPE_UNKNOWN) {
                    data->entity_collision_counts[type_a]++;
                    check_and_publish_entity_collision_count(data, type_a);
                }
            }
            
            if (event->result.entity_b_destroyed) {
                entity_type_t type_b = get_entity_type_from_ptr(event->entity_b);
                if (type_b != ENTITY_TYPE_UNKNOWN) {
                    data->entity_collision_counts[type_b]++;
                    check_and_publish_entity_collision_count(data, type_b);
                }
            }
            break;
            
        default:
            break;
    }
}

static void publish_collision_detected_event(collision_game_event_data_t* data, const collision_event_data_t* collision_event) {
    collision_detected_data_t* event_data = malloc(sizeof(collision_detected_data_t));
    if (!event_data) return;
    
    timestamp_ms_t now = get_clock_ticks_ms();
    timestamp_ms_t time_diff = now - data->last_rate_check_time;
    float entities_per_second = 0.0f;
    
    if (time_diff > 0) {
        entities_per_second = (float)data->total_collisions * 1000.0f / time_diff;
    }
    
    event_data->strategy_name = collision_event->strategy_name;
    event_data->total_count = data->total_collisions;
    event_data->entities_per_second = entities_per_second;
    
    game_event_t game_event = {
        .type = GAME_EVENT_COLLISION_DETECTED,
        .data = event_data,
        .data_size = sizeof(collision_detected_data_t)
    };
    
    publish(data->event_system, &game_event);
    free(event_data);
}

static void check_and_publish_high_collision_rate(collision_game_event_data_t* data) {
    timestamp_ms_t now = get_clock_ticks_ms();
    timestamp_ms_t time_since_last_check = now - data->last_rate_check_time;
    
    // Check collision rate every second
    if (time_since_last_check >= 1000) {
        size_t current_rate = data->total_collisions; // Simplified rate calculation
        
        if (current_rate >= data->config.high_collision_rate_threshold &&
            current_rate != data->last_collision_rate) {
            
            high_collision_rate_data_t* event_data = malloc(sizeof(high_collision_rate_data_t));
            if (event_data) {
                event_data->collision_rate = current_rate;
                event_data->threshold = data->config.high_collision_rate_threshold;
                
                game_event_t game_event = {
                    .type = GAME_EVENT_HIGH_COLLISION_RATE,
                    .data = event_data,
                    .data_size = sizeof(high_collision_rate_data_t)
                };
                
                publish(data->event_system, &game_event);
                free(event_data);
            }
        }
        
        data->last_collision_rate = current_rate;
        data->last_rate_check_time = now;
    }
}

static void check_and_publish_entity_collision_count(collision_game_event_data_t* data, entity_type_t entity_type) {
    if (entity_type >= ENTITY_TYPE_COUNT) return;
    
    size_t count = data->entity_collision_counts[entity_type];
    if (count >= data->config.entity_collision_count_threshold) {
        entity_collision_count_data_t* event_data = malloc(sizeof(entity_collision_count_data_t));
        if (event_data) {
            // Get entity type name
            const char* type_name = "Unknown";
            switch (entity_type) {
                case ENTITY_TYPE_DUCK: type_name = "Duck"; break;
                case ENTITY_TYPE_CRAB: type_name = "Crab"; break;
                case ENTITY_TYPE_BRICK: type_name = "Brick"; break;
                case ENTITY_TYPE_POPCORN: type_name = "Popcorn"; break;
                case ENTITY_TYPE_JELLYFISH: type_name = "Jellyfish"; break;
                default: break;
            }
            
            event_data->entity_type_name = type_name;
            event_data->collision_count = count;
            event_data->threshold = data->config.entity_collision_count_threshold;
            
            game_event_t game_event = {
                .type = GAME_EVENT_ENTITY_COLLISION_COUNT_REACHED,
                .data = event_data,
                .data_size = sizeof(entity_collision_count_data_t)
            };
            
            publish(data->event_system, &game_event);
            free(event_data);
        }
        
        // Reset count to avoid spamming events
        data->entity_collision_counts[entity_type] = 0;
    }
}

collision_game_event_config_t get_default_game_event_config(void) {
    collision_game_event_config_t config = {
        .high_collision_rate_threshold = 50,  // 50 collisions per second
        .entity_collision_count_threshold = 10,  // 10 collisions per entity type
        .publish_all_collisions = false  // Don't spam with every collision
    };
    return config;
}

const collision_game_event_data_t* get_game_event_observer_stats(collision_observer_t* observer) {
    if (!observer || !observer->user_data) return NULL;
    return (const collision_game_event_data_t*)observer->user_data;
}

void reset_game_event_observer_stats(collision_observer_t* observer) {
    collision_game_event_data_t* data = (collision_game_event_data_t*)observer->user_data;
    if (!data) return;
    
    data->total_collisions = 0;
    data->last_collision_rate = 0;
    data->last_rate_check_time = get_clock_ticks_ms();
    
    for (int i = 0; i < ENTITY_TYPE_COUNT; i++) {
        data->entity_collision_counts[i] = 0;
    }
}

void destroy_collision_game_event_observer(collision_observer_t* observer) {
    if (!observer) return;
    
    if (observer->user_data) {
        free(observer->user_data);
    }
    free(observer);
}
