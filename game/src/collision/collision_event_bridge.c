/**
 * @file collision_event_bridge.c
 * @brief Bridge between collision observer system and game event system
 */

#include "collision_event_bridge.h"
#include "geometry.h"
#include "crab.h"
#include "duck.h"
#include <stdlib.h>
#include <string.h>

// Forward declarations
static void bridge_observer_callback(collision_observer_t* observer, const collision_event_data_t* event);

// Global bridge instance for cleanup
static collision_observer_t* g_bridge_observer = NULL;

collision_observer_t* create_collision_event_bridge(event_system_ptr event_system, const char* name) {
    collision_observer_t* observer = malloc(sizeof(collision_observer_t));
    if (!observer) return NULL;
    
    collision_event_bridge_t* bridge = malloc(sizeof(collision_event_bridge_t));
    if (!bridge) {
        free(observer);
        return NULL;
    }
    
    // Initialize bridge data
    bridge->event_system = event_system;
    
    // Setup observer
    observer->on_collision_event = bridge_observer_callback;
    observer->observer_name = name;
    observer->user_data = bridge;
    observer->active = true;
    
    return observer;
}

static void bridge_observer_callback(collision_observer_t* observer, const collision_event_data_t* event) {
    collision_event_bridge_t* bridge = (collision_event_bridge_t*)observer->user_data;
    if (!bridge || !bridge->event_system) return;
    
    // Only bridge entity destruction events to game events
    if (event->type != COLLISION_EVENT_ENTITY_DESTROYED) {
        return;
    }
    
    // For this simplified bridge, we determine entity type based on collision strategy name
    // This avoids complex entity type detection and works with existing collision handlers
    
    if (event->result.entity_a_destroyed || event->result.entity_b_destroyed) {
        // Check strategy name to determine what kind of entities were involved
        
        // Crab destruction events
        if (strstr(event->strategy_name, "crab") != NULL) {
            // Try to get position from entity that was destroyed
            float x = 0, y = 0;
            
            if (event->result.entity_a_destroyed) {
                crab_ptr crab = (crab_ptr)event->entity_a;
                if (crab) {
                    x = crab->x;
                    y = crab->y;
                }
            } else if (event->result.entity_b_destroyed) {
                crab_ptr crab = (crab_ptr)event->entity_b;
                if (crab) {
                    x = crab->x;
                    y = crab->y;
                }
            }
            
            crab_destroyed_data_t* event_data = malloc(sizeof(crab_destroyed_data_t));
            if (event_data) {
                event_data->x = x;
                event_data->y = y;
                
                game_event_t game_event = {
                    .type = GAME_EVENT_CRAB_DESTROYED,
                    .data = event_data,
                    .data_size = sizeof(crab_destroyed_data_t)
                };
                
                publish(bridge->event_system, &game_event);
                free(event_data);
            }
        }
        
        // Duck death events
        if (strstr(event->strategy_name, "duck") != NULL) {
            // Try to get position from entity that was destroyed
            float x = 0, y = 0;
            
            if (event->result.entity_a_destroyed) {
                duck_ptr duck = (duck_ptr)event->entity_a;
                if (duck) {
                    x = duck->x;
                    y = duck->y;
                }
            } else if (event->result.entity_b_destroyed) {
                duck_ptr duck = (duck_ptr)event->entity_b;
                if (duck) {
                    x = duck->x;
                    y = duck->y;
                }
            }
            
            duck_died_data_t* event_data = malloc(sizeof(duck_died_data_t));
            if (event_data) {
                event_data->x = x;
                event_data->y = y;
                
                game_event_t game_event = {
                    .type = GAME_EVENT_DUCK_DIED,
                    .data = event_data,
                    .data_size = sizeof(duck_died_data_t)
                };
                
                publish(bridge->event_system, &game_event);
                free(event_data);
            }
        }
    }
}

void setup_collision_event_integration(game_ptr game) {
    collision_subject_t* subject = get_collision_subject();
    
    // Create bridge observer
    g_bridge_observer = create_collision_event_bridge(&game->event_system, "EventBridge");
    if (g_bridge_observer) {
        // Subscribe to entity destruction events
        collision_subject_subscribe(subject, COLLISION_EVENT_ENTITY_DESTROYED, g_bridge_observer);
    }
}

void cleanup_collision_event_integration(void) {
    if (g_bridge_observer) {
        collision_subject_t* subject = get_collision_subject();
        collision_subject_unsubscribe(subject, COLLISION_EVENT_ENTITY_DESTROYED, g_bridge_observer);
        destroy_collision_event_bridge(g_bridge_observer);
        g_bridge_observer = NULL;
    }
}

void destroy_collision_event_bridge(collision_observer_t* observer) {
    if (!observer) return;
    
    if (observer->user_data) {
        free(observer->user_data);
    }
    free(observer);
}
