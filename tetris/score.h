/*
 *  score.h
 *
 *  Created on: 11-Jan-2025
 *      Author: lucas-noce
 */

#ifndef _SCORE_H_
#define _SCORE_H_


/* ==========================================================================================================
 * Includes
 */

#include <stdint.h>


/* ==========================================================================================================
 * Typedefs
 */

/*!
  @brief        Indicates the game speed.
*/
typedef enum{
  GAME_SPEED_SLOWEST = 0,
  GAME_SPEED_SLOW,
  GAME_SPEED_MEDIUM,
  GAME_SPEED_FAST,
  GAME_SPEED_REALY_FAST,
  GAME_SPEED_FASTEST,
  GAME_SPEED_LAST_IDX,
} GAME_SPEEDS_E;

/*!
  @brief        Indicates the game difficulty.
*/
typedef enum{
  GAME_DIFFICULTY_EASY = 0,
  GAME_DIFFICULTY_MEDIUM,
  GAME_DIFFICULTY_HARD,
  GAME_DIFFICULTY_EXPERT,
  GAME_DIFFICULTY_LAST_IDX,
} GAME_DIFFICULTIES_E;



/* ==========================================================================================================
 * Global Functions
 */

void score_init( void );
void score_reset_to_zero( void );
void score_increment_speed( void );
int8_t score_set_difficulty( uint8_t game_difficulty );
uint8_t score_get_difficulty( void );
int8_t score_increment_complete_row( void );
int8_t score_increment_fix_piece( void );
void score_print( void );


#endif /* _SCORE_H_ */