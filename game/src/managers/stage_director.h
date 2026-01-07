/**
 * @file stage_director.h
 * @brief Dynamic stage lifecycle and transition management using State pattern
 *
 * Manages creation, initialization, transitions, and cleanup of game stages
 * without concrete dependencies on specific stage types. Uses a registry-based
 * approach to allow dynamic stage registration and lookup.
 */

#ifndef STAGE_DIRECTOR_H
#define STAGE_DIRECTOR_H

#include "game.h"
#include "stage.h"

/**
 * @brief Maximum number of stages that can be registered
 */
#define MAX_STAGES 8

/**
 * @brief Stage registry entry
 */
typedef struct {
    game_screen_t screen_type;
    stage_ptr (*create_stage_fn)(void);
    stage_ptr instance;
} stage_registry_entry_t;

/**
 * @brief Stage director context using State pattern
 */
typedef struct {
    stage_registry_entry_t stages[MAX_STAGES];
    size_t stage_count;
    stage_ptr current_stage;
    game_screen_t previous_screen;
} stage_director_t;

typedef stage_director_t* stage_director_ptr;

/**
 * @brief Initialize stage director
 * @param director Stage director to initialize
 * @param game Game context
 * @return true if initialization successful, false otherwise
 */
bool stage_director_init(stage_director_ptr director, game_ptr game);

/**
 * @brief Register a stage factory for a screen type
 * @param director Stage director
 * @param screen_type Screen type to associate with the stage
 * @param create_stage_fn Factory function to create the stage
 * @return true if registration successful, false otherwise
 */
bool stage_director_register_stage(stage_director_ptr director, 
                                   game_screen_t screen_type,
                                   stage_ptr (*create_stage_fn)(void));

/**
 * @brief Update current stage and handle transitions
 * @param director Stage director
 * @param game Game context
 * @return Stage action (PROGRESS or QUIT)
 */
game_stage_action_t stage_director_update(stage_director_ptr director, game_ptr game);

/**
 * @brief Clean up stage director and destroy all stages
 * @param director Stage director to clean up
 */
void stage_director_cleanup(stage_director_ptr director);

#endif // STAGE_DIRECTOR_H
