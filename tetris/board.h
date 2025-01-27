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

#include "pieces.h"

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
  @brief        Indicates the possible collision events of a piece in the board.
*/
typedef enum{
  BOARD_NO_COLLISION = 1,
  BOARD_COLLISION_BORDER_BOTTOM,
  BOARD_COLLISION_BORDER_LEFT,
  BOARD_COLLISION_BORDER_RIGHT,
  BOARD_COLLISION_OBJECT_BOTTOM,
  BOARD_COLLISION_OBJECT_LEFT,
  BOARD_COLLISION_OBJECT_RIGHT,
  BOARD_COLLISION_UNIDENTIFIED,
  BOARD_COLLISION_LAST_IDX,
} BOARD_COLLISIONS_E;

/*!
  @brief        Wrapper type used to indicate the board matrix.
*/
typedef uint8_t board_region_t;


/* ==========================================================================================================
 * Global Functions
 */

/*!
  @brief        Initializes the board with zeros and U-shaped border.

  @param        none

  @returns      void
*/
void board_init( void );

/*!
  @brief        Prints the board in its current state.

  @param        none

  @returns      void
*/
void board_print( void );

/*!
  @brief        Adds the bottom row of a piece to the top center of the board.

  @param[in]    type: one of the piece shape types (from PIECE_SHAPES_E).

  @returns      void
*/
void add_new_piece_to_board( uint8_t type );

/*!
  @brief        Moves a piece through the board, if said movement is valid.

  @param[in]    direction: which direction to move the piece (from BOARD_DIRECTIONS_E).

  @returns      One of the possible TETRIS_RET_x macro values (defined in main.h).
*/
int8_t move_current_piece_through_board( uint8_t direction );

/*!
  @brief        Moves a piece through the board, if said movement is valid.

  @param        none

  @returns      void
*/
void rotate_current_piece_through_board( void );

/*!
  @brief        Fix the current piece in its current position. After that, it can no longer be moved.

  @param        none

  @returns      void
*/
uint8_t fix_current_piece_on_board( void );

/*!
  @brief        Checks if a row has been completed, i.e. player has scored.

  @param        none

  @returns      TETRIS_GAME_OVER, TETRIS_GAME_NOT_OVER or TETRIS_GAME_WON.
*/
uint8_t check_complete_row( void );

#endif /* _BOARD_H_ */