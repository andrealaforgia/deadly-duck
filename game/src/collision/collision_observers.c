/**
 * @file collision_observers.c
 * @brief Concrete collision observers implementation
 */

#include "collision_observers.h"
#include "clock.h"
#include <stdlib.h>
#include <string.h>

// Forward declarations for observer callbacks
static void stats_observer_callback(collision_observer_t* observer, const collision_event_data_t* event);
static void debug_observer_callback(collision_observer_t* observer, const collision_event_data_t* event);
static void perf_observer_callback(collision_observer_t* observer, const collision_event_data_t* event);

// Global observer instances for cleanup
static collision_observer_t* g_stats_observer = NULL;
static collision_observer_t* g_debug_observer = NULL;
static collision_observer_t* g_perf_observer = NULL;

// Statistics Observer Implementation
collision_observer_t* create_collision_stats_observer(const char* name) {
    collision_observer_t* observer = malloc(sizeof(collision_observer_t));
    if (!observer) return NULL;
    
    collision_stats_data_t* stats = malloc(sizeof(collision_stats_data_t));
    if (!stats) {
        free(observer);
        return NULL;
    }
    
    // Initialize statistics
    memset(stats, 0, sizeof(collision_stats_data_t));
    stats->session_start_time = get_clock_ticks_ms();
    
    // Setup observer
    observer->on_collision_event = stats_observer_callback;
    observer->observer_name = name;
    observer->user_data = stats;
    observer->active = true;
    
    return observer;
}

static void stats_observer_callback(collision_observer_t* observer, const collision_event_data_t* event) {
    collision_stats_data_t* stats = (collision_stats_data_t*)observer->user_data;
    if (!stats) return;
    
    switch (event->type) {
        case COLLISION_EVENT_DETECTED:
            stats->total_collisions++;
            // Could track by strategy name hash if needed
            break;
            
        case COLLISION_EVENT_ENTITY_DESTROYED:
            stats->entities_destroyed++;
            break;
            
        case COLLISION_EVENT_POOL_RELEASED:
            stats->pool_releases++;
            break;
            
        default:
            break;
    }
}

// Debug Observer Implementation
collision_observer_t* create_collision_debug_observer(const char* name, 
                                                     FILE* log_file,
                                                     bool log_to_console,
                                                     bool verbose) {
    collision_observer_t* observer = malloc(sizeof(collision_observer_t));
    if (!observer) return NULL;
    
    collision_debug_data_t* debug = malloc(sizeof(collision_debug_data_t));
    if (!debug) {
        free(observer);
        return NULL;
    }
    
    // Initialize debug data
    debug->log_file = log_file;
    debug->log_to_console = log_to_console;
    debug->verbose_mode = verbose;
    debug->log_prefix = "[COLLISION]";
    
    // Setup observer
    observer->on_collision_event = debug_observer_callback;
    observer->observer_name = name;
    observer->user_data = debug;
    observer->active = true;
    
    return observer;
}

static void debug_observer_callback(collision_observer_t* observer, const collision_event_data_t* event) {
    collision_debug_data_t* debug = (collision_debug_data_t*)observer->user_data;
    if (!debug) return;
    
    const char* event_name = "";
    switch (event->type) {
        case COLLISION_EVENT_DETECTED: event_name = "DETECTED"; break;
        case COLLISION_EVENT_RESOLVED: event_name = "RESOLVED"; break;
        case COLLISION_EVENT_ENTITY_DESTROYED: event_name = "DESTROYED"; break;
        case COLLISION_EVENT_POOL_RELEASED: event_name = "RELEASED"; break;
        default: event_name = "UNKNOWN"; break;
    }
    
    char log_message[256];
    if (debug->verbose_mode) {
        snprintf(log_message, sizeof(log_message),
                "%s [%u] %s: %s - entities=%p,%p result=%d,%d,%d,%d\n",
                debug->log_prefix,
                event->timestamp,
                event_name,
                event->strategy_name,
                event->entity_a,
                event->entity_b,
                event->result.collision_detected,
                event->result.entity_a_destroyed,
                event->result.entity_b_destroyed,
                (int)(event->result.entity_a_modified || event->result.entity_b_modified));
    } else {
        snprintf(log_message, sizeof(log_message),
                "%s %s: %s\n",
                debug->log_prefix,
                event_name,
                event->strategy_name);
    }
    
    // Output to console if enabled
    if (debug->log_to_console) {
        printf("%s", log_message);
    }
    
    // Output to file if provided
    if (debug->log_file) {
        fprintf(debug->log_file, "%s", log_message);
        fflush(debug->log_file);
    }
}

// Performance Observer Implementation
collision_observer_t* create_collision_perf_observer(const char* name) {
    collision_observer_t* observer = malloc(sizeof(collision_observer_t));
    if (!observer) return NULL;
    
    collision_perf_data_t* perf = malloc(sizeof(collision_perf_data_t));
    if (!perf) {
        free(observer);
        return NULL;
    }
    
    // Initialize performance data
    timestamp_ms_t now = get_clock_ticks_ms();
    perf->last_collision_time = now;
    perf->collisions_per_second = 0;
    perf->collision_count_this_second = 0;
    perf->second_start_time = now;
    
    // Setup observer
    observer->on_collision_event = perf_observer_callback;
    observer->observer_name = name;
    observer->user_data = perf;
    observer->active = true;
    
    return observer;
}

static void perf_observer_callback(collision_observer_t* observer, const collision_event_data_t* event) {
    if (event->type != COLLISION_EVENT_DETECTED) return;
    
    collision_perf_data_t* perf = (collision_perf_data_t*)observer->user_data;
    if (!perf) return;
    
    timestamp_ms_t now = event->timestamp;
    perf->last_collision_time = now;
    
    // Check if we've moved to a new second
    if (now - perf->second_start_time >= 1000) {
        perf->collisions_per_second = perf->collision_count_this_second;
        perf->collision_count_this_second = 1;
        perf->second_start_time = now;
    } else {
        perf->collision_count_this_second++;
    }
}

// Utility Functions
const collision_stats_data_t* get_collision_stats(collision_observer_t* observer) {
    if (!observer || !observer->user_data) return NULL;
    return (const collision_stats_data_t*)observer->user_data;
}

void reset_collision_stats(collision_observer_t* observer) {
    if (!observer || !observer->user_data) return;
    
    collision_stats_data_t* stats = (collision_stats_data_t*)observer->user_data;
    memset(stats, 0, sizeof(collision_stats_data_t));
    stats->session_start_time = get_clock_ticks_ms();
}

void print_collision_stats(collision_observer_t* observer) {
    const collision_stats_data_t* stats = get_collision_stats(observer);
    if (!stats) return;
    
    timestamp_ms_t session_duration = get_clock_ticks_ms() - stats->session_start_time;
    
    printf("\n=== Collision Statistics ===\n");
    printf("Session duration: %u ms\n", session_duration);
    printf("Total collisions: %zu\n", stats->total_collisions);
    printf("Entities destroyed: %zu\n", stats->entities_destroyed);
    printf("Pool releases: %zu\n", stats->pool_releases);
    if (session_duration > 0) {
        printf("Collisions per second: %.2f\n", 
               (float)stats->total_collisions * 1000.0f / session_duration);
    }
    printf("============================\n\n");
}

size_t get_collision_rate(collision_observer_t* observer) {
    if (!observer || !observer->user_data) return 0;
    
    collision_perf_data_t* perf = (collision_perf_data_t*)observer->user_data;
    return perf->collisions_per_second;
}

void destroy_collision_observer(collision_observer_t* observer) {
    if (!observer) return;
    
    if (observer->user_data) {
        free(observer->user_data);
    }
    free(observer);
}

void setup_default_collision_observers(void) {
    collision_subject_t* subject = get_collision_subject();
    
    // Create and subscribe statistics observer
    g_stats_observer = create_collision_stats_observer("Statistics");
    if (g_stats_observer) {
        collision_subject_subscribe(subject, COLLISION_EVENT_DETECTED, g_stats_observer);
        collision_subject_subscribe(subject, COLLISION_EVENT_ENTITY_DESTROYED, g_stats_observer);
        collision_subject_subscribe(subject, COLLISION_EVENT_POOL_RELEASED, g_stats_observer);
    }
    
    // Create and subscribe debug observer (console only, non-verbose by default)
    g_debug_observer = create_collision_debug_observer("Debug", NULL, true, false);
    if (g_debug_observer) {
        collision_subject_subscribe(subject, COLLISION_EVENT_DETECTED, g_debug_observer);
        collision_subject_subscribe(subject, COLLISION_EVENT_RESOLVED, g_debug_observer);
    }
    
    // Create and subscribe performance observer
    g_perf_observer = create_collision_perf_observer("Performance");
    if (g_perf_observer) {
        collision_subject_subscribe(subject, COLLISION_EVENT_DETECTED, g_perf_observer);
    }
}

void cleanup_collision_observers(void) {
    collision_subject_t* subject = get_collision_subject();
    
    // Unsubscribe and destroy observers
    if (g_stats_observer) {
        collision_subject_unsubscribe(subject, COLLISION_EVENT_DETECTED, g_stats_observer);
        collision_subject_unsubscribe(subject, COLLISION_EVENT_ENTITY_DESTROYED, g_stats_observer);
        collision_subject_unsubscribe(subject, COLLISION_EVENT_POOL_RELEASED, g_stats_observer);
        destroy_collision_observer(g_stats_observer);
        g_stats_observer = NULL;
    }
    
    if (g_debug_observer) {
        collision_subject_unsubscribe(subject, COLLISION_EVENT_DETECTED, g_debug_observer);
        collision_subject_unsubscribe(subject, COLLISION_EVENT_RESOLVED, g_debug_observer);
        destroy_collision_observer(g_debug_observer);
        g_debug_observer = NULL;
    }
    
    if (g_perf_observer) {
        collision_subject_unsubscribe(subject, COLLISION_EVENT_DETECTED, g_perf_observer);
        destroy_collision_observer(g_perf_observer);
        g_perf_observer = NULL;
    }
    
    // Clear the subject
    collision_subject_clear(subject);
}
