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

  @returns      void
*/
static void _clear_board_entirely( void );

/*!
  @brief        Check if the piece will collide with another piece or the border after it is moved.

  @param[in]    direction: which direction to move the piece (from BOARD_DIRECTIONS_E).
  @param[in]    p_piece: pointer to the piece to be moved.

  @returns      Which direction to move the piece (from BOARD_DIRECTIONS_E).
*/
static uint8_t _check_piece_collision( uint8_t direction, PIECE_STRUCT_T *p_piece );

/*!
  @brief        Move the piece across the board by adding the piece's valid values to the
                corresponding board values at the next position.
                
  @param[in]    direction: which direction to move the piece (from BOARD_DIRECTIONS_E).
  @param[in]    p_piece: pointer to the piece to be moved.

  @returns      void
*/
static uint8_t _move_piece( uint8_t direction, PIECE_STRUCT_T *p_piece );


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


void add_new_piece_to_board( PIECE_STRUCT_T *p_piece ){
  uint8_t start_col       = BOARD_REGION_CENTER_COL - ( p_piece->order / 2 );
  uint8_t piece_idx       = 0;
  uint8_t piece_start_row = 0;

  p_piece->position_row   = 0;
  p_piece->position_col   = start_col;
  p_piece->displayed_rows = p_piece->order;
  p_piece->displayed_cols = p_piece->order;
  
  uint8_t i = 0;
  uint8_t j = 0;
  bool flag_break = false;

  /* Check for empty rows in the piece upper portion */
  while( i<p_piece->order && !flag_break ){
    while( j<p_piece->order ){
      piece_idx = ( p_piece->order * i ) + j;

      if( p_piece->shape[piece_idx] == 1 ){
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

  p_piece->position_row -= piece_start_row;

  uint8_t board_row_idx = 0;
  i = 0;
  j = 0;

  for( i=piece_start_row; i<p_piece->order; i++ ){
    for( j=0; j<p_piece->order; j++ ){
      piece_idx = ( p_piece->order * i ) + j;
      board[board_row_idx][start_col + j] = p_piece->shape[piece_idx];
    }
    board_row_idx++;
  }
}


uint8_t move_piece_through_board( uint8_t direction, PIECE_STRUCT_T *p_piece ){
  /*! TODO: debug this section of checking the hits */
  if( _check_piece_collision( direction, p_piece ) != BOARD_NO_COLLISION ){
    return TETRIS_RET_ERR;
  }

  /* Clear the piece in the current position */
  BOARD_AREA_T area;
  area.start_row = ( p_piece->position_row <= 0 ? 0 : p_piece->position_row );
  area.start_col = ( p_piece->position_col <= 0 ? 0 : p_piece->position_col );
  area.end_row   = p_piece->position_row + p_piece->order;  // this will be a problem
  area.end_col   = p_piece->position_col + p_piece->order;
  _clear_board_area( &area );

  /* Move the piece */
  return _move_piece( direction, p_piece );

  // return TETRIS_RET_OK;
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


static uint8_t _check_piece_collision( uint8_t direction, PIECE_STRUCT_T *p_piece ){
  uint8_t offset_row = 0;
  uint8_t offset_col = 0;
  uint8_t piece_idx  = 0;

  /* Check if piece will hit something */
  switch( direction ){
    case BOARD_DIRECTION_DOWN:
    {
      uint8_t collision_result = 0;

      for( int8_t j=(p_piece->order-1); j>=0; j-- ){    // col
        for( int8_t i=(p_piece->order-1); i>=0; i-- ){  // row
          piece_idx = ( p_piece->order * i ) + j;
          LOG_DBG( "p_piece->shape[%u][%u]: %u\n", i, j, p_piece->shape[piece_idx] );

          if( p_piece->shape[piece_idx] == 1 ){  // this cell can hit something
            offset_col = p_piece->position_col + j;
            offset_row = p_piece->position_row + i + 1;  // one row below
            collision_result = board[offset_row][offset_col] + p_piece->shape[piece_idx];
            
            LOG_DBG( "board[%u][%u]: %u\n", offset_row, offset_col, board[offset_row][offset_col] );

            if( collision_result == 2 ){
              LOG_INF( "*** piece hit another piece at the bottom ***\n" );
              return BOARD_COLLISION_OBJECT_BOTTOM;
            }
            else if( collision_result > 3 ){
              LOG_INF( "*** piece hit bottom border ***\n" );
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

      for( int8_t i=0; i<p_piece->order; i++ ){    // row
        for( int8_t j=0; j<p_piece->order; j++ ){  // col
          piece_idx = ( p_piece->order * i ) + j;
          LOG_DBG( "p_piece->shape[%u][%u]: %u\n", i, j, p_piece->shape[piece_idx] );

          if( p_piece->shape[piece_idx] == 1 ){  // this cell can hit something
            offset_col = p_piece->position_col + j - 1;  // one col to the left
            offset_row = p_piece->position_row + i;
            collision_result = board[offset_row][offset_col] + p_piece->shape[piece_idx];
            
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

      for( int8_t i=0; i<p_piece->order; i++ ){         // row
        for( int8_t j=(p_piece->order-1); j>=0; j-- ){  // col
          piece_idx = ( p_piece->order * i ) + j;
          LOG_DBG( "p_piece->shape[%u][%u]: %u\n", i, j, p_piece->shape[piece_idx] );

          if( p_piece->shape[piece_idx] == 1 ){  // this cell can hit something
            offset_col = p_piece->position_col + j + 1;  // one col to the right
            offset_row = p_piece->position_row + i;
            collision_result = board[offset_row][offset_col] + p_piece->shape[piece_idx];
            
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


static uint8_t _move_piece( uint8_t direction, PIECE_STRUCT_T *p_piece ){
  int8_t horizontal_direction = BOARD_H_DISPLACEMENT_RIGHT;
  uint8_t offset_row          = 0;
  uint8_t offset_col          = 0;
  uint8_t piece_idx           = 0;

  switch( direction ){
    case BOARD_DIRECTION_DOWN:
    {
      uint8_t piece_start_row = 0;
      
      if( p_piece->displayed_rows < p_piece->order ){
        offset_row = p_piece->position_row;
        piece_start_row = p_piece->order - p_piece->displayed_rows - 1;
      }
      else{
        offset_row = p_piece->position_row + 1;
        piece_start_row = p_piece->order - p_piece->displayed_rows;
      }

      for( uint8_t i=piece_start_row; i<p_piece->order; i++ ){
        for( uint8_t j=0; j<p_piece->order; j++ ){
          offset_col = p_piece->position_col + j;
          piece_idx  = (p_piece->order * i) + j;
          board[offset_row][offset_col] += p_piece->shape[piece_idx];
        }
        offset_row++;
      }

      if( p_piece->displayed_rows < p_piece->order ) p_piece->displayed_rows++;
      else p_piece->position_row++;

      break;
    }

    case BOARD_DIRECTION_LEFT:
      horizontal_direction = BOARD_H_DISPLACEMENT_LEFT;

    case BOARD_DIRECTION_RIGHT:
    {
      // if( (int8_t) p_piece->position_col + horizontal_direction <= 0 ||
      //              p_piece->position_col + horizontal_direction >= ( BOARD_COL_SIZE - 1 ) ){
      //   printf( "aaaaaaaaa\n" );
      //   return TETRIS_RET_ERR;  // error
      // }

      uint8_t piece_start_row = p_piece->order - p_piece->displayed_rows;
      // uint8_t piece_start_col;

      // if( p_piece->displayed_cols != p_piece->order ){
      //   piece_start_col = 
      // }
      offset_row = p_piece->position_row;

      for( uint8_t i=piece_start_row; i<p_piece->order; i++ ){
        for( uint8_t j=0; j<p_piece->displayed_cols; j++ ){
          offset_col = p_piece->position_col + j + horizontal_direction;
          piece_idx  = ( p_piece->order * i ) + j;
          board[offset_row][offset_col] += p_piece->shape[piece_idx];
        }
        offset_row++;
      }

      p_piece->position_col += horizontal_direction;
      break;
    }

    case BOARD_DIRECTION_LAST_IDX:
    default:
      return TETRIS_RET_ERR;
  }

  return TETRIS_RET_OK;
}
