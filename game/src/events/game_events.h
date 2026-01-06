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
    GAME_EVENT_DUCK_DIED
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

#endif  // GAME_SRC_EVENTS_GAME_EVENTS_H_
