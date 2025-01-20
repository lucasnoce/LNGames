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
  @brief        Indicates the possible types/shape of pieces.
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


/* ==========================================================================================================
 * Global Functions
 */

void score_reset_to_zero( void );
int8_t score_increment( uint8_t game_speed );
void score_print( void );


#endif /* _SCORE_H_ */