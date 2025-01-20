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


/* ==========================================================================================================
 * Static variables
 */

static uint32_t game_score     = 0;
static uint8_t game_speed      = 0;
static uint8_t game_difficulty = 0;

static const uint32_t score_table[GAME_SPEED_LAST_IDX][GAME_DIFFICULTY_LAST_IDX] = {
  { 10,  15,  20,  30 },
  { 20,  25,  35,  40 },
  { 40,  50,  60,  70 },
  { 80,  90,  110, 130 },
  { 160, 175, 200, 250 },
  { 320, 400, 500, 700 }
};

static const uint8_t score_table_fix_piece[GAME_SPEED_LAST_IDX] = {
  1,
  2,
  4,
  8,
  16,
  32
};


/* ==========================================================================================================
 * Global Functions Declaration
 */

void score_init( void ){
  game_score = 0;
  game_speed = GAME_SPEED_SLOWEST;
  game_difficulty = GAME_DIFFICULTY_EASY;
}


void score_reset_to_zero( void ){
  game_score = 0;
  game_speed = GAME_SPEED_SLOWEST;
}


void score_increment_speed( void ){
  game_speed += ( game_speed < ( GAME_SPEED_LAST_IDX - 1) ? 1 : 0 );
}


int8_t score_set_difficulty( uint8_t difficulty ){
  if( game_difficulty >= GAME_DIFFICULTY_LAST_IDX ){
    return TETRIS_RET_ERR;
  }

  game_difficulty = difficulty;
  return TETRIS_RET_OK;
}

uint8_t score_get_difficulty( void ){
  return game_difficulty;
}


int8_t score_increment_complete_row( void ){
  if( game_speed >= GAME_SPEED_LAST_IDX || game_difficulty >= GAME_DIFFICULTY_LAST_IDX ){
    return TETRIS_RET_ERR;
  }

  game_score += score_table[game_speed][game_difficulty];
  return TETRIS_RET_OK;
}


int8_t score_increment_fix_piece( void ){
  if( game_speed >= GAME_SPEED_LAST_IDX ){
    return TETRIS_RET_ERR;
  }

  game_score += score_table_fix_piece[game_speed];
  return TETRIS_RET_OK;
}


void score_print( void ){
  LOG_GAME( "\nScore: %u\n\n", game_score );
}
