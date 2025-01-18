/*
 *  board.c
 *
 *  Created on: 11-Jan-2025
 *      Author: lucas-noce
 */

/* ==========================================================================================================
 * Includes
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "pieces.h"
#include "board.h"


/* ==========================================================================================================
 * Definitions
 */

/*
  Board is a rectangle matrix of sizes BOARD_PLAYABLE_ROW_SIZE x BOARD_PLAYABLE_COL_SIZE with a U-shaped border, as follows:

  3 0 0 ... 0 3
  3 0 0 ... 0 3
  3 0 0 ... 0 3
  : : :     : :
  3 0 0 ... 0 3
  3 3 3 ... 3 3
*/
#define BOARD_PLAYABLE_ROW_SIZE   8
#define BOARD_PLAYABLE_COL_SIZE   12

#define BOARD_PLAYABLE_OFFSET     PIECE_LARGEST_MATRIX_ORDER
#define BOARD_PLAYABLE_START_ROW  BOARD_PLAYABLE_OFFSET
#define BOARD_PLAYABLE_START_COL  BOARD_PLAYABLE_OFFSET
#define BOARD_PLAYABLE_END_ROW    BOARD_PLAYABLE_OFFSET + BOARD_PLAYABLE_ROW_SIZE
#define BOARD_PLAYABLE_END_COL    BOARD_PLAYABLE_OFFSET + BOARD_PLAYABLE_COL_SIZE

// #define BOARD_ROW_SIZE            ( BOARD_PLAYABLE_COL_SIZE + ( 2 * BOARD_PLAYABLE_OFFSET ) )
// #define BOARD_COL_SIZE            ( BOARD_PLAYABLE_COL_SIZE + ( 2 * BOARD_PLAYABLE_OFFSET ) )
#define BOARD_ROW_SIZE            BOARD_PLAYABLE_ROW_SIZE
#define BOARD_COL_SIZE            BOARD_PLAYABLE_COL_SIZE

#define BOARD_REGION_CENTER_COL   6
#define BOARD_REGION_BORDER_VALUE 3

#define BOARD_H_DISPLACEMENT_RIGHT  ( (int8_t)  1 )
#define BOARD_H_DISPLACEMENT_LEFT   ( (int8_t) -1 )


/* ==========================================================================================================
 * Static Typedefs
 */

typedef struct BOARD_AREA_TAG{
  uint8_t start_row;
  uint8_t start_col;
  uint8_t end_row;
  uint8_t end_col;
} BOARD_AREA_T;


/* ==========================================================================================================
 * Static variables
 */

/* Board start on top left corner */
static board_region_t board[BOARD_ROW_SIZE][BOARD_COL_SIZE] = { 0 };

static PIECE_STRUCT_T current_piece;
static PIECE_STRUCT_T *p_current_piece;


/* ==========================================================================================================
 * Static Function Prototypes
 */

/*!
  @brief        Clears a portion of the board, specified by the BOARD_AREA_T parameter.

  @param[in]    p_area: pointer to the area to be cleared.

  @returns      void
*/
static void _clear_board_area( BOARD_AREA_T *p_area );

/*!
  @brief        Clears the entire board, populating the matrix with zeros.

  @param        none

  @returns      void
*/
static void _clear_board_entirely( void );

/*!
  @brief        Clears a portion of the board, specified by the piece position.

  @param        none

  @returns      void
*/
static void _remove_current_piece_from_board( void );

/*!
  @brief        Sets a specific value to all positions where the piece value is 1.

  @param[in]    value: the value to write on the valid positions.

  @returns      void
*/
static void _set_current_piece_value_to_board( uint8_t value );

/*!
  @brief        Check if the piece will collide with another piece or the border after it is moved.

  @param[in]    direction: which direction to move the piece (from BOARD_DIRECTIONS_E).

  @returns      Which direction to move the piece (from BOARD_DIRECTIONS_E).
*/
static uint8_t _check_current_piece_collision( uint8_t direction );

/*!
  @brief        Move the piece across the board by adding the piece's valid values to the
                corresponding board values at the next position.
                
  @param[in]    direction: which direction to move the piece (from BOARD_DIRECTIONS_E).

  @returns      void
*/
static int8_t _move_current_piece( uint8_t direction );


/* ==========================================================================================================
 * Global Functions Declaration
 */

void board_init( void ){
  _clear_board_entirely();

  /* Board has U-shaped border */
  for( uint8_t i=0; i<BOARD_ROW_SIZE; i++ ){
    board[i][0]                    = BOARD_REGION_BORDER_VALUE;
    board[i][ BOARD_COL_SIZE - 1 ] = BOARD_REGION_BORDER_VALUE;
  }

  for( uint8_t j=0; j<BOARD_COL_SIZE; j++ ){
    board[ BOARD_ROW_SIZE - 1 ][j] = BOARD_REGION_BORDER_VALUE;
  }
  
  /* Test with piece portions: */
  // board[1][3] = 1;
  // board[3][6] = 1;
  // board[1][9] = 1;
}


void board_print( void ){
  for( uint8_t i=0; i<BOARD_ROW_SIZE; i++ ){
    for( uint8_t j=0; j<BOARD_COL_SIZE; j++ ){
      if( j == ( BOARD_COL_SIZE - 1 ) ){
        LOG_INF( "%u\n", board[i][j] );
      }
      else{
        if( board[i][j] != 0 )
          LOG_INF( "%u ", board[i][j] );
        else
          LOG_INF( "_|" );
      }
    }
  }
}


void add_new_piece_to_board( uint8_t type ){
  p_current_piece = &current_piece;
  piece_get( type, p_current_piece );

  uint8_t start_col       = BOARD_REGION_CENTER_COL - ( current_piece.order / 2 );
  uint8_t piece_idx       = 0;
  uint8_t piece_start_row = 0;

  current_piece.position_row   = 0;
  current_piece.position_col   = start_col;
  current_piece.displayed_rows = current_piece.order;
  current_piece.displayed_cols = current_piece.order;
  
  uint8_t i = 0;
  uint8_t j = 0;
  bool flag_break = false;

  /* Check for empty rows in the piece upper portion */
  while( i<current_piece.order && !flag_break ){
    while( j<current_piece.order ){
      piece_idx = ( current_piece.order * i ) + j;

      if( current_piece.shape[piece_idx] == 1 ){
        LOG_DBG( "i: %u", i );
        piece_start_row = i;
        flag_break = true;
        break;
      }

      j++;
    }

    j = 0;
    i++;
  }

  current_piece.position_row -= piece_start_row;

  uint8_t board_row_idx = 0;
  i = 0;
  j = 0;

  for( i=piece_start_row; i<current_piece.order; i++ ){
    for( j=0; j<current_piece.order; j++ ){
      piece_idx = ( current_piece.order * i ) + j;
      board[board_row_idx][start_col + j] = current_piece.shape[piece_idx];
    }
    board_row_idx++;
  }
}


int8_t move_current_piece_through_board( uint8_t direction ){
  if( p_current_piece == NULL )
    return TETRIS_RET_ERR_NO_PIECE;

  /*! TODO: debug this section of checking the hits */
  uint8_t ret = _check_current_piece_collision( direction );

  if( ret != BOARD_NO_COLLISION ){
    return (int8_t) ret;
  }

  /* Clear the piece in the current position */
  _remove_current_piece_from_board();

  /* Move the piece */
  return _move_current_piece( direction );

  // return TETRIS_RET_OK;
}

uint8_t fix_current_piece_on_board( void ){
  if( p_current_piece == NULL )
    return TETRIS_RET_ERR_NO_PIECE;

  if( !current_piece.is_moving ){
    _set_current_piece_value_to_board( 1 );
    p_current_piece = NULL;
    return TETRIS_RET_READY;
  }
  else{
    if( current_piece.is_colliding ){
      current_piece.is_moving = false;
    }
    else{
      current_piece.is_moving = true;
    }

    return TETRIS_RET_OK;
  }
}


/* ==========================================================================================================
 * Static Functions Declaration
 */


static void _clear_board_area( BOARD_AREA_T *p_area ){
  for( uint8_t i=p_area->start_row; i<p_area->end_row; i++ ){
    for( uint8_t j=p_area->start_col; j<p_area->end_col; j++ ){
      if( board[i][j] != BOARD_REGION_BORDER_VALUE )
        board[i][j] = 0;
    }
  }
}


static void _clear_board_entirely( void ){
  BOARD_AREA_T area = { 0, 0, BOARD_ROW_SIZE, BOARD_COL_SIZE };
  _clear_board_area( &area );
}


static void _remove_current_piece_from_board( void ){
  _set_current_piece_value_to_board( 0 );
}


static void _set_current_piece_value_to_board( uint8_t value ){
  uint8_t piece_idx = 0;
  uint8_t piece_row = 0;
  uint8_t board_row = 0;
  uint8_t board_col = 0;

  for( uint8_t i=0; i<current_piece.order; i++ ){
    piece_row = ( current_piece.order * i );
    board_row = current_piece.position_row + i;

    for( uint8_t j=0; j<current_piece.order; j++ ){
      piece_idx = piece_row + j;
      board_col = current_piece.position_col + j;

      if( current_piece.shape[piece_idx] != 0 ){
        board[board_row][board_col] = value;
      }
    }
  }
}


static uint8_t _check_current_piece_collision( uint8_t direction ){
  uint8_t offset_row = 0;
  uint8_t offset_col = 0;
  uint8_t piece_idx  = 0;

  current_piece.is_colliding = false;

  /* Check if piece will hit something */
  switch( direction ){
    case BOARD_DIRECTION_DOWN:
    {
      uint8_t collision_result = 0;

      for( int8_t j=(current_piece.order-1); j>=0; j-- ){    // col
        for( int8_t i=(current_piece.order-1); i>=0; i-- ){  // row
          piece_idx = ( current_piece.order * i ) + j;
          LOG_DBG( "current_piece.shape[%u][%u]: %u\n", i, j, current_piece.shape[piece_idx] );

          if( current_piece.shape[piece_idx] == 1 ){  // this cell can hit something
            offset_col = current_piece.position_col + j;
            offset_row = current_piece.position_row + i + 1;  // one row below
            collision_result = board[offset_row][offset_col] + current_piece.shape[piece_idx];
            
            LOG_DBG( "board[%u][%u]: %u\n", offset_row, offset_col, board[offset_row][offset_col] );

            if( collision_result == 2 ){
              LOG_INF( "*** piece hit another piece at the bottom ***\n" );
              current_piece.is_colliding = true;
              return BOARD_COLLISION_OBJECT_BOTTOM;
            }
            else if( collision_result > 3 ){
              LOG_INF( "*** piece hit bottom border ***\n" );
              current_piece.is_colliding = true;
              return BOARD_COLLISION_BORDER_BOTTOM;
            }

            break;
          }
        }
      }
      
      break;
    }

    case BOARD_DIRECTION_LEFT:
    {
      uint8_t collision_result = 0;

      for( int8_t i=0; i<current_piece.order; i++ ){    // row
        for( int8_t j=0; j<current_piece.order; j++ ){  // col
          piece_idx = ( current_piece.order * i ) + j;
          LOG_DBG( "current_piece.shape[%u][%u]: %u\n", i, j, current_piece.shape[piece_idx] );

          if( current_piece.shape[piece_idx] == 1 ){  // this cell can hit something
            offset_col = current_piece.position_col + j - 1;  // one col to the left
            offset_row = current_piece.position_row + i;
            collision_result = board[offset_row][offset_col] + current_piece.shape[piece_idx];
            
            LOG_DBG( "board[%u][%u]: %u\n", offset_row, offset_col, board[offset_row][offset_col] );

            if( collision_result == 2 ){
              LOG_INF( "*** piece hit another piece to the left ***\n" );
              return BOARD_COLLISION_OBJECT_LEFT;
            }
            else if( collision_result > 3 ){
              LOG_INF( "*** piece hit left-side border ***\n" );
              return BOARD_COLLISION_BORDER_LEFT;
            }

            break;
          }
        }
      }
      
      break;
    }

    case BOARD_DIRECTION_RIGHT:
    {
      uint8_t collision_result = 0;

      for( int8_t i=0; i<current_piece.order; i++ ){         // row
        for( int8_t j=(current_piece.order-1); j>=0; j-- ){  // col
          piece_idx = ( current_piece.order * i ) + j;
          LOG_DBG( "current_piece.shape[%u][%u]: %u\n", i, j, current_piece.shape[piece_idx] );

          if( current_piece.shape[piece_idx] == 1 ){  // this cell can hit something
            offset_col = current_piece.position_col + j + 1;  // one col to the right
            offset_row = current_piece.position_row + i;
            collision_result = board[offset_row][offset_col] + current_piece.shape[piece_idx];
            
            LOG_DBG( "board[%u][%u]: %u\n", offset_row, offset_col, board[offset_row][offset_col] );

            if( collision_result == 2 ){
              LOG_INF( "*** piece hit another piece to the right ***\n" );
              return BOARD_COLLISION_OBJECT_RIGHT;
            }
            else if( collision_result > 3 ){
              LOG_INF( "*** piece hit right-side border ***\n" );
              return BOARD_COLLISION_BORDER_RIGHT;
            }

            break;
          }
        }
      }
      
      break;
    }

    case BOARD_DIRECTION_LAST_IDX:
    default:
      return BOARD_COLLISION_UNIDENTIFIED;
  }

  return BOARD_NO_COLLISION;
}


static int8_t _move_current_piece( uint8_t direction ){
  int8_t horizontal_direction = BOARD_H_DISPLACEMENT_RIGHT;
  uint8_t offset_row          = 0;
  uint8_t offset_col          = 0;
  uint8_t piece_idx           = 0;

  switch( direction ){
    case BOARD_DIRECTION_DOWN:
    {
      uint8_t piece_start_row = 0;
      
      if( current_piece.displayed_rows < current_piece.order ){
        offset_row = current_piece.position_row;
        piece_start_row = current_piece.order - current_piece.displayed_rows - 1;
      }
      else{
        offset_row = current_piece.position_row + 1;
        piece_start_row = current_piece.order - current_piece.displayed_rows;
      }

      for( uint8_t i=piece_start_row; i<current_piece.order; i++ ){
        for( uint8_t j=0; j<current_piece.order; j++ ){
          offset_col = current_piece.position_col + j;
          piece_idx  = (current_piece.order * i) + j;
          board[offset_row][offset_col] += current_piece.shape[piece_idx];
        }
        offset_row++;
      }

      if( current_piece.displayed_rows < current_piece.order ) current_piece.displayed_rows++;
      else current_piece.position_row++;

      break;
    }

    case BOARD_DIRECTION_LEFT:
      horizontal_direction = BOARD_H_DISPLACEMENT_LEFT;

    case BOARD_DIRECTION_RIGHT:
    {
      // if( (int8_t) current_piece.position_col + horizontal_direction <= 0 ||
      //              current_piece.position_col + horizontal_direction >= ( BOARD_COL_SIZE - 1 ) ){
      //   printf( "aaaaaaaaa\n" );
      //   return TETRIS_RET_ERR;  // error
      // }

      uint8_t piece_start_row = current_piece.order - current_piece.displayed_rows;
      // uint8_t piece_start_col;

      // if( current_piece.displayed_cols != current_piece.order ){
      //   piece_start_col = 
      // }
      offset_row = current_piece.position_row;

      for( uint8_t i=piece_start_row; i<current_piece.order; i++ ){
        for( uint8_t j=0; j<current_piece.displayed_cols; j++ ){
          offset_col = current_piece.position_col + j + horizontal_direction;
          piece_idx  = ( current_piece.order * i ) + j;
          board[offset_row][offset_col] += current_piece.shape[piece_idx];
        }
        offset_row++;
      }

      current_piece.position_col += horizontal_direction;
      break;
    }

    case BOARD_DIRECTION_LAST_IDX:
    default:
      return TETRIS_RET_ERR;
  }

  return TETRIS_RET_OK;
}
