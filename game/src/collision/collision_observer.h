/**
 * @file collision_observer.h
 * @brief Observer pattern for collision event notifications
 *
 * Provides an observer-based system for collision events, allowing multiple
 * components to subscribe to and respond to collision events without tight coupling.
 * This enables features like statistics tracking, sound effects, visual effects,
 * and scoring to respond to collisions independently.
 */

#ifndef COLLISION_OBSERVER_H
#define COLLISION_OBSERVER_H

#include "collision_interface.h"
#include "types.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Maximum number of observers per event type
 */
#define MAX_COLLISION_OBSERVERS 16

/**
 * @brief Collision event types for observer notifications
 */
typedef enum {
    COLLISION_EVENT_DETECTED,       // Collision was detected
    COLLISION_EVENT_RESOLVED,       // Collision response completed
    COLLISION_EVENT_ENTITY_DESTROYED, // Entity was destroyed in collision
    COLLISION_EVENT_POOL_RELEASED,  // Entity was released from pool
    COLLISION_EVENT_COUNT
} collision_event_type_t;

/**
 * @brief Collision event data
 */
typedef struct {
    collision_event_type_t type;
    entity_ptr entity_a;
    entity_ptr entity_b;
    collision_result_t result;
    game_ptr game;
    const char* strategy_name;
    timestamp_ms_t timestamp;
} collision_event_data_t;

/**
 * @brief Forward declaration for collision observer
 */
typedef struct collision_observer_t collision_observer_t;

/**
 * @brief Collision observer callback function
 * @param observer Observer instance
 * @param event Event data
 */
typedef void (*collision_observer_callback_t)(collision_observer_t* observer, const collision_event_data_t* event);

/**
 * @brief Collision observer interface
 */
struct collision_observer_t {
    collision_observer_callback_t on_collision_event;
    const char* observer_name;
    void* user_data;  // Custom data for observer
    bool active;      // Whether observer is currently active
};

/**
 * @brief Collision subject (observable) that manages observers
 */
typedef struct {
    collision_observer_t* observers[COLLISION_EVENT_COUNT][MAX_COLLISION_OBSERVERS];
    size_t observer_counts[COLLISION_EVENT_COUNT];
    bool notifications_enabled;
} collision_subject_t;

/**
 * @brief Initialize collision subject
 * @param subject Subject to initialize
 */
void collision_subject_init(collision_subject_t* subject);

/**
 * @brief Subscribe observer to collision events
 * @param subject Collision subject
 * @param event_type Type of events to observe
 * @param observer Observer to subscribe
 * @return true if subscription successful
 */
bool collision_subject_subscribe(collision_subject_t* subject, 
                                collision_event_type_t event_type,
                                collision_observer_t* observer);

/**
 * @brief Unsubscribe observer from collision events
 * @param subject Collision subject
 * @param event_type Type of events to unsubscribe from
 * @param observer Observer to unsubscribe
 * @return true if unsubscription successful
 */
bool collision_subject_unsubscribe(collision_subject_t* subject,
                                  collision_event_type_t event_type,
                                  collision_observer_t* observer);

/**
 * @brief Notify all observers of a collision event
 * @param subject Collision subject
 * @param event Event data to broadcast
 */
void collision_subject_notify(collision_subject_t* subject, const collision_event_data_t* event);

/**
 * @brief Enable/disable collision event notifications
 * @param subject Collision subject
 * @param enabled Whether notifications should be enabled
 */
void collision_subject_set_notifications(collision_subject_t* subject, bool enabled);

/**
 * @brief Clear all observers from subject
 * @param subject Collision subject to clear
 */
void collision_subject_clear(collision_subject_t* subject);

/**
 * @brief Create collision event data
 * @param type Event type
 * @param entity_a First entity
 * @param entity_b Second entity
 * @param result Collision result
 * @param game Game state
 * @param strategy_name Name of collision strategy
 * @return Initialized event data
 */
collision_event_data_t collision_event_create(collision_event_type_t type,
                                              entity_ptr entity_a,
                                              entity_ptr entity_b,
                                              collision_result_t result,
                                              game_ptr game,
                                              const char* strategy_name);

/**
 * @brief Get global collision subject instance
 * @return Global collision subject
 */
collision_subject_t* get_collision_subject(void);

#endif // COLLISION_OBSERVER_H
