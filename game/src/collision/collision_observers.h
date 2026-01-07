/**
 * @file collision_observers.h
 * @brief Concrete collision observers for common use cases
 *
 * Provides pre-built observers for typical collision-related functionality
 * such as statistics tracking, debug logging, and performance monitoring.
 */

#ifndef COLLISION_OBSERVERS_H
#define COLLISION_OBSERVERS_H

#include "collision_observer.h"
#include "types.h"
#include <stdio.h>

/**
 * @brief Statistics tracking observer data
 */
typedef struct {
    size_t total_collisions;
    size_t collisions_by_strategy[32];  // Track by strategy (limited)
    size_t entities_destroyed;
    size_t pool_releases;
    timestamp_ms_t session_start_time;
} collision_stats_data_t;

/**
 * @brief Debug logging observer data
 */
typedef struct {
    FILE* log_file;
    bool log_to_console;
    bool verbose_mode;
    const char* log_prefix;
} collision_debug_data_t;

/**
 * @brief Performance monitoring observer data
 */
typedef struct {
    timestamp_ms_t last_collision_time;
    size_t collisions_per_second;
    size_t collision_count_this_second;
    timestamp_ms_t second_start_time;
} collision_perf_data_t;

/**
 * @brief Create statistics tracking observer
 * @param name Observer name
 * @return Initialized statistics observer
 */
collision_observer_t* create_collision_stats_observer(const char* name);

/**
 * @brief Create debug logging observer
 * @param name Observer name
 * @param log_file File to log to (NULL for console only)
 * @param log_to_console Whether to also log to console
 * @param verbose Whether to use verbose logging
 * @return Initialized debug observer
 */
collision_observer_t* create_collision_debug_observer(const char* name, 
                                                     FILE* log_file,
                                                     bool log_to_console,
                                                     bool verbose);

/**
 * @brief Create performance monitoring observer
 * @param name Observer name
 * @return Initialized performance observer
 */
collision_observer_t* create_collision_perf_observer(const char* name);

/**
 * @brief Get statistics from stats observer
 * @param observer Stats observer
 * @return Statistics data (read-only)
 */
const collision_stats_data_t* get_collision_stats(collision_observer_t* observer);

/**
 * @brief Reset statistics in stats observer
 * @param observer Stats observer
 */
void reset_collision_stats(collision_observer_t* observer);

/**
 * @brief Print statistics report to console
 * @param observer Stats observer
 */
void print_collision_stats(collision_observer_t* observer);

/**
 * @brief Get current collision rate from performance observer
 * @param observer Performance observer
 * @return Collisions per second
 */
size_t get_collision_rate(collision_observer_t* observer);

/**
 * @brief Destroy collision observer and free memory
 * @param observer Observer to destroy
 */
void destroy_collision_observer(collision_observer_t* observer);

/**
 * @brief Setup default collision observers
 * Automatically creates and subscribes common observers to the global subject
 */
void setup_default_collision_observers(void);

/**
 * @brief Cleanup all collision observers
 * Removes all observers from global subject and frees memory
 */
void cleanup_collision_observers(void);

#endif // COLLISION_OBSERVERS_H
