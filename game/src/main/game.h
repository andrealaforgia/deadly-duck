/**
 * @file game.h
 * @brief Core game state and lifecycle management
 *
 * Provides the central game structure and lifecycle functions (init/terminate).
 * This is the main entry point for game initialization and cleanup.
 */

#ifndef GAME_MAIN_GAME_H_
#define GAME_MAIN_GAME_H_

#include "types.h"
#include <stdbool.h>

#include "audio.h"
#include "bitmap_font.h"
#include "event_system.h"
#include "graphics.h"
#include "keyboard.h"
#include "object_pool.h"
#include "texture.h"

// Entity modules
#include "brick.h"
#include "crab.h"
#include "duck.h"
#include "jellyfish.h"
#include "popcorn.h"

// Forward declarations for stage system
typedef struct stage_t stage_t;

/**
 * Stage action returned by stage update functions
 */
typedef enum {
    PROGRESS, // Continue with current stage
    QUIT      // Quit the game
} game_stage_action_t;

/**
 * Game screen states (will be replaced by stage system)
 */
typedef enum { SCREEN_TRIBUTE, SCREEN_COVER, SCREEN_GAME, SCREEN_GAME_OVER } game_screen_t;

// Sound effect indices for audio context
#define SOUND_QUACK 0
#define SOUND_CRAB_HIT 1
#define SOUND_BRICK_DROP 2
#define SOUND_DUCK_DEATH 3
#define NUM_SOUNDS 4

/**
 * Main game state structure
 * Contains all game data including graphics, audio, entities, and game state
 */
typedef struct {
    // Core systems
    graphics_context_t graphics_context;
    audio_context_t audio_context;
    event_system_t event_system;
    keyboard_state_t keyboard_state;

    // Resources
    texture_t sprite_sheet;
    texture_t cover_image;
    int cover_width;
    int cover_height;
    bitmap_font_t font;

    // Game state
    bool running;
    game_screen_t current_screen;
    stage_t *current_stage; // Current active stage (for stage system)

    // Tribute screen state
    float scroll_y;
    timestamp_ms_t tribute_start_time;
    bool tribute_waiting; // True if waiting for space key before scrolling

    // Game over screen state
    float game_over_y; // Y position of GAME OVER text

    // Game entities
    duck_t duck;

    // Object pools for efficient entity management
    object_pool_t popcorn_pool;
    object_pool_t crab_pool;
    object_pool_t brick_pool;
    object_pool_t jellyfish_pool;

    // Game statistics
    int lives;
    int score;
} game_t;

// Pointer typedef for game
typedef game_t *game_ptr;

/**
 * Initialize the game
 * Sets up graphics, audio, loads resources, and initializes game state
 *
 * @param game Pointer to game structure to initialize
 * @return true if initialization successful, false otherwise
 */
bool game_init(game_t *game);

/**
 * Clean up and terminate the game
 * Frees all resources and shuts down SDL subsystems
 *
 * @param game Pointer to game structure to terminate
 */
void game_terminate(game_t *game);

#endif // GAME_MAIN_GAME_H_
