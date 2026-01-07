/**
 * @file game_events.h
 * @brief Game event type definitions and data structures
 *
 * Defines all game-specific event types like crab destroyed and duck died.
 * Includes event data structures that carry information about each event
 * for subscribers to process.
 */

#ifndef GAME_SRC_EVENTS_GAME_EVENTS_H_
#define GAME_SRC_EVENTS_GAME_EVENTS_H_

#include <stdbool.h>

// Game event types
typedef enum {
    GAME_EVENT_CRAB_DESTROYED = 0,
    GAME_EVENT_DUCK_DIED,
    GAME_EVENT_COLLISION_DETECTED,
    GAME_EVENT_HIGH_COLLISION_RATE,
    GAME_EVENT_ENTITY_COLLISION_COUNT_REACHED
} game_event_type_t;

// Event data structures
typedef struct {
    float x;
    float y;
} crab_destroyed_data_t;

typedef struct {
    float x;
    float y;
} duck_died_data_t;

// Collision event data structures
typedef struct {
    const char *strategy_name;
    size_t total_count;
    float entities_per_second;
} collision_detected_data_t;

typedef struct {
    size_t collision_rate;
    size_t threshold;
} high_collision_rate_data_t;

typedef struct {
    const char *entity_type_name;
    size_t collision_count;
    size_t threshold;
} entity_collision_count_data_t;

#endif // GAME_SRC_EVENTS_GAME_EVENTS_H_
