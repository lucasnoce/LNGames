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

#include "pieces.h"
#include "board.h"


/* ==========================================================================================================
 * Definitions
 */

#define BOARD_ROW_SIZE            20
#define BOARD_COL_SIZE            12
#define BOARD_REGION_CENTER_COL   6
#define BOARD_REGION_BORDER_VALUE 3


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

static void _clear_board_area( BOARD_AREA_T *p_area );


/* ==========================================================================================================
 * Global Functions Declaration
 */

void board_init( void ){
  /* Board is U-shaped */
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
    board[0][start_col + j] = p_piece->format[piece_idx];
  }
}

uint8_t move_piece_through_board( uint8_t direction, PIECE_STRUCT_T *p_piece ){
  uint8_t offset_row = 0;
  uint8_t offset_col = 0;
  uint8_t piece_idx  = 0;

  /*! TODO: debug this section of checking the hits */

  /* Check if piece will hit something */
  switch( direction ){
    case BOARD_DIRECTION_DOWN:
    {
      uint8_t piece_row = p_piece->order * ( p_piece->order - 1 );
      offset_row = p_piece->position_row + p_piece->displayed_rows + 1;
      
      for( uint8_t j=0; j<p_piece->order; j++ ){
        offset_col = p_piece->position_col + j;
        piece_idx  = piece_row + j;
        
        if( board[offset_row][offset_col] + p_piece->format[piece_idx] > 1 ){
          // hit
          printf( "*** piece hit something ***\n" );
          return PIECE_RET_ERR;
        }
      }
      break;
    }

    case BOARD_DIRECTION_RIGHT:
    {
      uint8_t piece_col = p_piece->order;
      uint8_t piece_start_row = p_piece->order - p_piece->displayed_rows;
      offset_col = p_piece->position_col + p_piece->order + 1;

      for( uint8_t i=piece_start_row; i<p_piece->order; i++ ){
        offset_row = p_piece->position_row + i;
        piece_idx  = ( p_piece->order * i ) + piece_col;
        
        if( board[offset_row][offset_col] + p_piece->format[piece_idx] > 1 ){
          // hit
          printf( "*** piece hit something ***\n" );
          return PIECE_RET_ERR;
        }
      }
      break;
    }

    case BOARD_DIRECTION_LEFT:
    {
      uint8_t piece_start_row = p_piece->order - p_piece->displayed_rows;
      offset_col = p_piece->position_col - 1;

      for( uint8_t i=piece_start_row; i<p_piece->order; i++ ){
        offset_row = p_piece->position_row + i;
        piece_idx  = ( p_piece->order * i );
        
        if( board[offset_row][offset_col] + p_piece->format[piece_idx] > 1 ){
          // hit
          printf( "*** piece hit something ***\n" );
          return PIECE_RET_ERR;
        }
      }
      break;
    }

    case BOARD_DIRECTION_LAST_IDX:
    default:
      return PIECE_RET_ERR;
  }


  /* Clear the piece in the current position */
  BOARD_AREA_T area;
  area.start_row = p_piece->position_row;
  area.start_col = p_piece->position_col;
  area.end_row   = p_piece->position_row + p_piece->displayed_rows;  // this will be a problem
  area.end_col   = p_piece->position_col + p_piece->order;
  _clear_board_area( &area );


  /* Move the piece */
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
          board[offset_row][offset_col] = p_piece->format[piece_idx];
        }
        offset_row++;
      }

      if( p_piece->displayed_rows < p_piece->order ) p_piece->displayed_rows++;
      else p_piece->position_row++;

      break;
    }

    case BOARD_DIRECTION_RIGHT:
    {
      uint8_t piece_start_row = 0;
      offset_row = p_piece->position_row;
      piece_start_row = p_piece->order - p_piece->displayed_rows;

      for( uint8_t i=piece_start_row; i<p_piece->order; i++ ){
        for( uint8_t j=0; j<p_piece->order; j++ ){
          offset_col = p_piece->position_col + j + 1;
          piece_idx  = (p_piece->order * i) + j;
          board[offset_row][offset_col] = p_piece->format[piece_idx];
        }
        offset_row++;
      }

      p_piece->position_col++;
      break;
    }

    case BOARD_DIRECTION_LEFT:
    {
      uint8_t piece_start_row = 0;
      offset_row = p_piece->position_row;
      piece_start_row = p_piece->order - p_piece->displayed_rows;

      for( uint8_t i=piece_start_row; i<p_piece->order; i++ ){
        for( uint8_t j=0; j<p_piece->order; j++ ){
          offset_col = p_piece->position_col + j - 1;
          piece_idx  = (p_piece->order * i) + j;
          board[offset_row][offset_col] = p_piece->format[piece_idx];
        }
        offset_row++;
      }
      
      p_piece->position_col--;
      break;
    }

    case BOARD_DIRECTION_LAST_IDX:
    default:
      return PIECE_RET_ERR;
  }

  return PIECE_RET_OK;
}


/* ==========================================================================================================
 * Static Functions Declaration
 */

static void _clear_board_area( BOARD_AREA_T *p_area ){
  for( uint8_t i=p_area->start_row; i<p_area->end_row; i++ ){
    for( uint8_t j=p_area->start_col; j<p_area->end_col; j++ ){
      board[i][j] = 0;
    }
  }
}