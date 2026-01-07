/**
 * @file score.h
 * @brief Scoring system and point values
 *
 * Defines point values for destroying different entities and provides
 * functions for updating player score.
 */

#ifndef GAME_SRC_SCORING_SCORE_H_
#define GAME_SRC_SCORING_SCORE_H_

#include "game.h"

// Score limits
#define MAX_SCORE 999999

// Point values for different actions
#define CRAB_HIT_SCORE 30        // Points awarded for destroying a crab
#define SURVIVAL_BONUS_SCORE 10  // Bonus points for surviving time periods
#define LEVEL_COMPLETE_BONUS 100 // Bonus points for completing a level

/**
 * Award points for hitting a crab
 *
 * @param game Game state
 */
void score_crab_hit(game_ptr game);

/**
 * Subscribe to game events for scoring
 *
 * @param game Game state
 */
void subscribe_score_events(game_ptr game);

#endif // GAME_SRC_SCORING_SCORE_H_
