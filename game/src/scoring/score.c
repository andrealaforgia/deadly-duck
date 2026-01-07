/**
 * @file score.c
 * @brief Scoring system implementation
 */

#include "score.h"
#include "event_system.h"
#include "game.h"
#include "game_events.h"

static void add_score(game_ptr game, int points) {
    if (game->score + points > MAX_SCORE) {
        game->score = MAX_SCORE;
        return;
    }
    game->score += points;
}

void score_crab_hit(game_ptr game) { add_score(game, CRAB_HIT_SCORE); }

static void on_crab_destroyed(const game_event_t *event, void *user_data) {
    (void)event; // Unused - event data not needed for scoring
    game_ptr game = (game_ptr)user_data;
    add_score(game, CRAB_HIT_SCORE);
}

void subscribe_score_events(game_ptr game) {
    subscribe(&game->event_system, GAME_EVENT_CRAB_DESTROYED, on_crab_destroyed, game);
}
