/*
 *  score.c
 *
 *  Created on: 19-Jan-2025
 *      Author: lucas-noce
 */

/* ==========================================================================================================
 * Includes
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "main.h"
#include "score.h"

static volatile uint32_t game_score = 0;

static const uint8_t score_table[GAME_SPEED_LAST_IDX] = {
  10,
  20,
  40,
  80,
  160,
  320
};

void score_reset_to_zero( void ){
  game_score = 0;
}

int8_t score_increment( uint8_t game_speed ){
  if( game_speed >= GAME_SPEED_LAST_IDX ){
    return TETRIS_RET_ERR;
  }

  game_score += score_table[game_speed];
  return TETRIS_RET_OK;
}

void score_print( void ){
  LOG_GAME( "\n\nScore: %u", game_score );
}
