/**
 * @file collision_observer.c
 * @brief Observer pattern implementation for collision events
 */

#include "collision_observer.h"
#include "clock.h"
#include <string.h>

// Global collision subject instance
static collision_subject_t global_collision_subject;
static bool subject_initialized = false;

void collision_subject_init(collision_subject_t* subject) {
    if (!subject) return;
    
    // Initialize all observer arrays to NULL
    for (int i = 0; i < COLLISION_EVENT_COUNT; i++) {
        for (int j = 0; j < MAX_COLLISION_OBSERVERS; j++) {
            subject->observers[i][j] = NULL;
        }
        subject->observer_counts[i] = 0;
    }
    
    subject->notifications_enabled = true;
}

bool collision_subject_subscribe(collision_subject_t* subject, 
                                collision_event_type_t event_type,
                                collision_observer_t* observer) {
    if (!subject || !observer || event_type >= COLLISION_EVENT_COUNT) {
        return false;
    }
    
    // Check if we have room for more observers
    if (subject->observer_counts[event_type] >= MAX_COLLISION_OBSERVERS) {
        return false;
    }
    
    // Check if observer is already subscribed
    for (size_t i = 0; i < subject->observer_counts[event_type]; i++) {
        if (subject->observers[event_type][i] == observer) {
            return false; // Already subscribed
        }
    }
    
    // Add observer to the list
    size_t count = subject->observer_counts[event_type];
    subject->observers[event_type][count] = observer;
    subject->observer_counts[event_type]++;
    
    return true;
}

bool collision_subject_unsubscribe(collision_subject_t* subject,
                                  collision_event_type_t event_type,
                                  collision_observer_t* observer) {
    if (!subject || !observer || event_type >= COLLISION_EVENT_COUNT) {
        return false;
    }
    
    // Find the observer in the list
    for (size_t i = 0; i < subject->observer_counts[event_type]; i++) {
        if (subject->observers[event_type][i] == observer) {
            // Remove observer by shifting remaining observers down
            for (size_t j = i; j < subject->observer_counts[event_type] - 1; j++) {
                subject->observers[event_type][j] = subject->observers[event_type][j + 1];
            }
            subject->observers[event_type][subject->observer_counts[event_type] - 1] = NULL;
            subject->observer_counts[event_type]--;
            return true;
        }
    }
    
    return false; // Observer not found
}

void collision_subject_notify(collision_subject_t* subject, const collision_event_data_t* event) {
    if (!subject || !event || !subject->notifications_enabled) {
        return;
    }
    
    if (event->type >= COLLISION_EVENT_COUNT) {
        return;
    }
    
    // Notify all observers for this event type
    for (size_t i = 0; i < subject->observer_counts[event->type]; i++) {
        collision_observer_t* observer = subject->observers[event->type][i];
        if (observer && observer->active && observer->on_collision_event) {
            observer->on_collision_event(observer, event);
        }
    }
}

void collision_subject_set_notifications(collision_subject_t* subject, bool enabled) {
    if (subject) {
        subject->notifications_enabled = enabled;
    }
}

void collision_subject_clear(collision_subject_t* subject) {
    if (!subject) return;
    
    // Clear all observer lists
    for (int i = 0; i < COLLISION_EVENT_COUNT; i++) {
        for (int j = 0; j < MAX_COLLISION_OBSERVERS; j++) {
            subject->observers[i][j] = NULL;
        }
        subject->observer_counts[i] = 0;
    }
}

collision_event_data_t collision_event_create(collision_event_type_t type,
                                              entity_ptr entity_a,
                                              entity_ptr entity_b,
                                              collision_result_t result,
                                              game_ptr game,
                                              const char* strategy_name) {
    collision_event_data_t event = {
        .type = type,
        .entity_a = entity_a,
        .entity_b = entity_b,
        .result = result,
        .game = game,
        .strategy_name = strategy_name ? strategy_name : "Unknown",
        .timestamp = get_clock_ticks_ms()
    };
    
    return event;
}

collision_subject_t* get_collision_subject(void) {
    if (!subject_initialized) {
        collision_subject_init(&global_collision_subject);
        subject_initialized = true;
    }
    
    return &global_collision_subject;
}
