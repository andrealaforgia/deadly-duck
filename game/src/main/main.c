#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include "constants.h"
#include "game.h"
#include "stage_manager.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    game_t game = {0};

    if (!game_init(&game)) {
        game_terminate(&game);
        return 1;
    }

    printf("Deadly Duck - Press ESC to quit\n");
    printf("Sprite sheet dimensions: %dx%d (%.2f:1 ratio)\n",
           game.sprite_sheet.width, game.sprite_sheet.height,
           (float)game.sprite_sheet.width / game.sprite_sheet.height);

    // Initialize stage manager
    stage_manager_t stage_manager = {0};
    if (!stage_manager_init(&stage_manager, &game)) {
        game_terminate(&game);
        return 1;
    }

    // Game loop
    Uint32 frame_start;
    int frame_time;

    while (game.running) {
        frame_start = SDL_GetTicks();

        // Update stages and handle transitions
        game_stage_action_t action = stage_manager_update(&stage_manager, &game);

        if (action == QUIT) {
            game.running = false;
        }

        // Frame rate limiting
        frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    // Cleanup
    stage_manager_cleanup(&stage_manager);
    game_terminate(&game);
    return 0;
}
