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

/*!
  @brief        Indicates the possible directions a piece can be moved.
*/
typedef enum{
  BOARD_DIRECTION_DOWN = 0,
  BOARD_DIRECTION_LEFT,
  BOARD_DIRECTION_RIGHT,
  BOARD_DIRECTION_LAST_IDX,
} BOARD_DIRECTIONS_E;

/*!
  @brief        Wrapper type used to indicate the board matrix.
*/
typedef uint8_t board_region_t;


/* ==========================================================================================================
 * Global Functions
 */

/*!
  @brief        Initializes the board with zeros and U-shaped border.

  @returns      void
*/
void board_init( void );

/*!
  @brief        Prints the board in its current state.

  @returns      void
*/
void board_print( void );

/*!
  @brief        Adds the bottom row of a piece to the top center of the board.

  @param[in]    p_piece: pointer to the piece to be added.

  @returns      void
*/
void add_new_piece_to_board( PIECE_STRUCT_T *p_piece );

/*!
  @brief        Moves a piece through the board, if said movement is valid.

  @param[in]    direction: which direction to move the piece (from BOARD_DIRECTIONS_E).
  @param[in]    p_piece: pointer to the piece to be moved.

  @returns      One of the possible TETRIS_RET_x macro values (defined in main.h).
*/
uint8_t move_piece_through_board( uint8_t direction, PIECE_STRUCT_T *p_piece );


#endif /* _BOARD_H_ */