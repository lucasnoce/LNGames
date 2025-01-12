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

#define BOARD_ROW_SIZE            20
#define BOARD_COL_SIZE            12
#define BOARD_REGION_CENTER_COL   6
#define BOARD_REGION_BORDER_VALUE 3

#define BOARD_H_DISPLACEMENT_RIGHT  ( (int8_t) 1 )
#define BOARD_H_DISPLACEMENT_LEFT   ( (int8_t) -1 )

#define BOARD_PIECE_COLLISION     true
#define BOARD_PIECE_NO_COLLISION  false


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

  @returns      Either BOARD_PIECE_COLLISION or BOARD_PIECE_NO_COLLISION.
*/
static bool _check_piece_collision( uint8_t direction, PIECE_STRUCT_T *p_piece );

/*!
  @brief        Move the piece across the board by adding the piece's valid values to the
                corresponding board values at the next position.
                
  @param[in]    direction: which direction to move the piece (from BOARD_DIRECTIONS_E).
  @param[in]    p_piece: pointer to the piece to be moved.

  @returns      void
*/
static void _move_piece( uint8_t direction, PIECE_STRUCT_T *p_piece );


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
      if( j == ( BOARD_COL_SIZE - 1 ) )
        printf( "%u\n", board[i][j] );
      else
        if( board[i][j] != 0 )
          printf( "%u ", board[i][j] );
        else
          printf( "_|" );
    }
  }
}


void add_new_piece_to_board( PIECE_STRUCT_T *p_piece ){
  uint8_t start_col = BOARD_REGION_CENTER_COL - ( p_piece->order / 2 );
  uint8_t piece_row = p_piece->order * ( p_piece->order - 1 );
  uint8_t piece_idx = 0;

  p_piece->position_col   = start_col;
  p_piece->position_row   = 0;
  p_piece->displayed_rows = 1;

  for( uint8_t j=0; j<p_piece->order; j++ ){
    piece_idx  = piece_row + j;
    board[0][start_col + j] = p_piece->shape[piece_idx];
  }
}


uint8_t move_piece_through_board( uint8_t direction, PIECE_STRUCT_T *p_piece ){
  /*! TODO: debug this section of checking the hits */
  if( _check_piece_collision( direction, p_piece ) != BOARD_PIECE_NO_COLLISION ){
    return TETRIS_RET_ERR;
  }

  /* Clear the piece in the current position */
  BOARD_AREA_T area;
  area.start_row = p_piece->position_row;
  area.start_col = p_piece->position_col;
  area.end_row   = p_piece->position_row + p_piece->displayed_rows;  // this will be a problem
  area.end_col   = p_piece->position_col + p_piece->order;
  _clear_board_area( &area );

  /* Move the piece */
  _move_piece( direction, p_piece );

  return TETRIS_RET_OK;
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


static bool _check_piece_collision( uint8_t direction, PIECE_STRUCT_T *p_piece ){
  uint8_t offset_row = 0;
  uint8_t offset_col = 0;
  uint8_t piece_idx  = 0;

  /* Check if piece will hit something */
  switch( direction ){
    case BOARD_DIRECTION_DOWN:
    {
      uint8_t piece_row = p_piece->order * ( p_piece->order - 1 );
      offset_row = p_piece->position_row + p_piece->displayed_rows + 1;
      
      for( uint8_t j=0; j<p_piece->order; j++ ){
        offset_col = p_piece->position_col + j;
        piece_idx  = piece_row + j;
        
        if( board[offset_row][offset_col] + p_piece->shape[piece_idx] > 1 ){
          // hit
          printf( "*** piece hit something ***\n" );
          return BOARD_PIECE_COLLISION;
        }
      }
      break;
    }

    case BOARD_DIRECTION_RIGHT:
    {
      uint8_t piece_col = p_piece->order - 1;
      uint8_t piece_start_row = p_piece->order - p_piece->displayed_rows;
      offset_col = p_piece->position_col + p_piece->order;
      offset_row = p_piece->position_row;

      for( uint8_t i=piece_start_row; i<p_piece->order; i++ ){
        piece_idx  = ( p_piece->order * i ) + piece_col;
        uint8_t collision_result = board[offset_row][offset_col] + p_piece->shape[piece_idx];
        
        // printf( "%u + %u = %u\n",
        //   board[offset_row][offset_col],
        //   p_piece->shape[piece_idx],
        //   collision_result );

        /* 
          if collision_result == 3, then it's just an empty portion
          of the piece touching the border.
        */
        if( collision_result == 2 ){
          printf( "*** piece hit other piece ***\n" );
          return BOARD_PIECE_COLLISION;
        }
        else if( collision_result > 3 ){
          printf( "*** piece hit border ***\n" );
          return BOARD_PIECE_COLLISION;
        }
        
        offset_row++;
      }
      break;
    }

    case BOARD_DIRECTION_LEFT:
    {
      uint8_t piece_col = 0;
      uint8_t piece_start_row = p_piece->order - p_piece->displayed_rows;
      offset_col = p_piece->position_col - 1;
      offset_row = p_piece->position_row;

      for( uint8_t i=piece_start_row; i<p_piece->order; i++ ){
        piece_idx  = ( p_piece->order * i ) + piece_col;
        uint8_t collision_result = board[offset_row][offset_col] + p_piece->shape[piece_idx];
        
        printf( "%u + %u = %u\n",
          board[offset_row][offset_col],
          p_piece->shape[piece_idx],
          collision_result );

        /* 
          if collision_result == 3, then it's just an empty portion
          of the piece touching the border.
        */
        if( collision_result == 2 ){
          printf( "*** piece hit other piece ***\n" );
          return BOARD_PIECE_COLLISION;
        }
        else if( collision_result > 3 ){
          printf( "*** piece hit border ***\n" );
          return BOARD_PIECE_COLLISION;
        }
        
        offset_row++;
      }

      break;
    }

    case BOARD_DIRECTION_LAST_IDX:
    default:
      return BOARD_PIECE_COLLISION;
  }

  return BOARD_PIECE_NO_COLLISION;
}


static void _move_piece( uint8_t direction, PIECE_STRUCT_T *p_piece ){
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
      uint8_t piece_start_row = 0;
      offset_row = p_piece->position_row;
      piece_start_row = p_piece->order - p_piece->displayed_rows;

      for( uint8_t i=piece_start_row; i<p_piece->order; i++ ){
        for( uint8_t j=0; j<p_piece->order; j++ ){
          offset_col = p_piece->position_col + j + horizontal_direction;
          piece_idx  = (p_piece->order * i) + j;
          board[offset_row][offset_col] += p_piece->shape[piece_idx];
        }
        offset_row++;
      }

      p_piece->position_col += horizontal_direction;
      break;
    }

    case BOARD_DIRECTION_LAST_IDX:
    default:
      return;
  }
}
