/*
 *  board.h
 *
 *  Created on: 11-Jan-2025
 *      Author: lucas-noce
 */

#ifndef _BOARD_H_
#define _BOARD_H_


/* ==========================================================================================================
 * Includes
 */

#include <stdint.h>


/* ==========================================================================================================
 * Definitions
 */

/* ==========================================================================================================
 * Typedefs
 */

typedef enum{
  BOARD_DIRECTION_DOWN = 0,
  BOARD_DIRECTION_LEFT,
  BOARD_DIRECTION_RIGHT,
  BOARD_DIRECTION_LAST_IDX,
} BOARD_DIRECTIONS_E;

typedef uint8_t board_region_t;


/* ==========================================================================================================
 * Global Functions
 */

void board_init( void );
void board_print( void );
void add_new_piece_to_board( PIECE_STRUCT_T *p_piece );
uint8_t move_piece_through_board( uint8_t direction, PIECE_STRUCT_T *p_piece );


#endif /* _BOARD_H_ */