/**
 * @file collision_event_bridge.h
 * @brief Bridge between collision observer system and game event system
 *
 * Provides integration between the specialized collision observer pattern
 * and the general game event system. Allows collision events to be
 * automatically translated and published to the game event system.
 */

#ifndef COLLISION_EVENT_BRIDGE_H
#define COLLISION_EVENT_BRIDGE_H

#include "collision_observer.h"
#include "event_system.h"
#include "game_events.h"
#include "game.h"

/**
 * @brief Bridge observer that translates collision events to game events
 */
typedef struct {
    collision_observer_t base;
    event_system_ptr event_system;
} collision_event_bridge_t;

/**
 * @brief Create collision-to-game event bridge
 * @param event_system Game event system to publish to
 * @param name Observer name for debugging
 * @return Initialized bridge observer
 */
collision_observer_t* create_collision_event_bridge(event_system_ptr event_system, const char* name);

/**
 * @brief Setup collision event bridge integration
 * @param game Game instance containing event system
 * 
 * This function:
 * 1. Creates a bridge observer
 * 2. Subscribes it to relevant collision events
 * 3. Ensures collision events are automatically published to game event system
 */
void setup_collision_event_integration(game_ptr game);

/**
 * @brief Cleanup collision event bridge
 * Removes bridge observer and cleans up integration
 */
void cleanup_collision_event_integration(void);

/**
 * @brief Destroy collision event bridge observer
 * @param observer Bridge observer to destroy
 */
void destroy_collision_event_bridge(collision_observer_t* observer);

#endif // COLLISION_EVENT_BRIDGE_H
