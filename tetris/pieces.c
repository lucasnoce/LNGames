/*
 *  pieces.c
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
#include "game_config.h"
#include "pieces.h"


/* ==========================================================================================================
 * Definitions
 */

/* ==========================================================================================================
 * Static Typedefs
 */

/* ==========================================================================================================
 * Static variables
 */

static piece_shape_t piece_square[PIECE_SQUARE_SIZE] = {
  1, 1,
  1, 1
};

static piece_shape_t piece_T[PIECE_T_SIZE] = {
  0, 0, 0,
  0, 1, 0,
  1, 1, 1
};

static piece_shape_t piece_line[PIECE_LINE_SIZE] = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  1, 1, 1, 1
};

static piece_shape_t piece_Z[PIECE_Z_SIZE] = {
  0, 0, 0,
  1, 1, 0,
  0, 1, 1
};

static piece_shape_t piece_Z_flipped[PIECE_Z_FLIPPED_SIZE] = {
  0, 0, 0,
  0, 1, 1,
  1, 1, 0
};

static piece_shape_t piece_L[PIECE_L_SIZE] = {
  0, 0, 0,
  0, 0, 1,
  1, 1, 1
};

static piece_shape_t piece_L_flipped[PIECE_L_FLIPPED_SIZE] = {
  0, 0, 0,
  1, 0, 0,
  1, 1, 1
};


/* ==========================================================================================================
 * Static Function Prototypes
 */

/*!
  @brief        Copies the template piece shape into the piece struct shape.

  @param[out]   dst: pointer to the piece struct.
  @param[in]    src: pointer to the template piece.
  @param[in]    size: the number of elements to be copied, depends on dst size.

  @returns      One of the possible TETRIS_RET_x macro values (defined in main.h).
*/
void _piece_copy_shape( PIECE_STRUCT_T *dst, piece_shape_t *src, uint8_t size );

/*!
  @brief        Checks if a row or column (segment) of the piece is completely empty, i.e. every element is 0.

  @param[in]    p_piece: pointer to the piece to be checked.
  @param[in]    seg_idx: the index of the segment to be checked.
  @param[in]    check_row: whether the segment to be checked is a row (true) or a column (false).

  @returns      true if segment is empty, false otherwise.
*/
static inline bool _piece_is_segment_empty( PIECE_STRUCT_T *p_piece, uint8_t seg_idx, bool check_row );


/* ==========================================================================================================
 * Global Functions Declaration
 */


int8_t piece_get( uint8_t type, PIECE_STRUCT_T *p_piece ){
  if( p_piece == NULL )
    return TETRIS_RET_ERR_NO_PIECE;
  
  p_piece->is_colliding = false;
  p_piece->is_moving    = true;
  
  switch( type ){
    case PIECE_SHAPE_SQUARE:
      p_piece->order       = PIECE_SQUARE_MATRIX_ORDER;
      p_piece->size        = PIECE_SQUARE_SIZE;
      p_piece->print_color = GAME_CONFIG_PRINT_BOARD_PIECE_SQUARE_COLOR;
      _piece_copy_shape( p_piece, piece_square, p_piece->size );
      break;

    case PIECE_SHAPE_T:
      p_piece->order       = PIECE_T_MATRIX_ORDER;
      p_piece->size        = PIECE_T_SIZE;
      p_piece->print_color = GAME_CONFIG_PRINT_BOARD_PIECE_T_COLOR;
      _piece_copy_shape( p_piece, piece_T, p_piece->size );
      break;

    case PIECE_SHAPE_LINE:
      p_piece->order       = PIECE_LINE_MATRIX_ORDER;
      p_piece->size        = PIECE_LINE_SIZE;
      p_piece->print_color = GAME_CONFIG_PRINT_BOARD_PIECE_LINE_COLOR;
      _piece_copy_shape( p_piece, piece_line, p_piece->size );
      break;

    case PIECE_SHAPE_Z:
      p_piece->order       = PIECE_Z_MATRIX_ORDER;
      p_piece->size        = PIECE_Z_SIZE;
      p_piece->print_color = GAME_CONFIG_PRINT_BOARD_PIECE_Z_COLOR;
      _piece_copy_shape( p_piece, piece_Z, p_piece->size );
      break;

    case PIECE_SHAPE_Z_FLIPPED:
      p_piece->order       = PIECE_Z_FLIPPED_MATRIX_ORDER;
      p_piece->size        = PIECE_Z_FLIPPED_SIZE;
      p_piece->print_color = GAME_CONFIG_PRINT_BOARD_PIECE_Z_FLIPPED_COLOR;
      _piece_copy_shape( p_piece, piece_Z_flipped, p_piece->size );
      break;

    case PIECE_SHAPE_L:
      p_piece->order       = PIECE_L_MATRIX_ORDER;
      p_piece->size        = PIECE_L_SIZE;
      p_piece->print_color = GAME_CONFIG_PRINT_BOARD_PIECE_L_COLOR;
      _piece_copy_shape( p_piece, piece_L, p_piece->size );
      break;

    case PIECE_SHAPE_L_FLIPPED:
      p_piece->order       = PIECE_L_FLIPPED_MATRIX_ORDER;
      p_piece->size        = PIECE_L_FLIPPED_SIZE;
      p_piece->print_color = GAME_CONFIG_PRINT_BOARD_PIECE_L_FLIPPED_COLOR;
      _piece_copy_shape( p_piece, piece_L_flipped, p_piece->size );
      break;

    case PIECE_SHAPE_LAST_IDX:
    default:
      return TETRIS_RET_ERR;
  }

  return TETRIS_RET_OK;
}


int8_t piece_rotate_90deg( PIECE_STRUCT_T *p_piece ){
  piece_shape_t temp[PIECE_LARGEST_SIZE] = { 0 };
  uint8_t temp_idx = 0;
  uint8_t piece_idx = 0;

  if( p_piece == NULL )
    return TETRIS_RET_ERR_NO_PIECE;

  for( uint8_t i=0; i<p_piece->order; i++ ){
    for( uint8_t j=0; j<p_piece->order; j++ ){
      piece_idx = (p_piece->order * i) + j;
      temp_idx  = (p_piece->order * j) + ( p_piece->order - 1 - i );
      temp[temp_idx] = p_piece->shape[piece_idx];
    }
  }

  for( uint8_t i=0; i<p_piece->size; i++ ){
    p_piece->shape[i] = temp[i];
  }

  return TETRIS_RET_OK;
}


int8_t piece_print( PIECE_STRUCT_T *p_piece ){
  uint8_t piece_idx = 0;

  if( p_piece == NULL )
    return TETRIS_RET_ERR_NO_PIECE;

  for( uint8_t i=0; i<p_piece->order; i++ ){
    for( uint8_t j=0; j<p_piece->order; j++ ){
      piece_idx = (p_piece->order * i) + j;
      if( j == ( p_piece->order - 1 ) )
        LOG_GAME( "%u\n", p_piece->shape[piece_idx] );
      else
        LOG_GAME( "%u, ", p_piece->shape[piece_idx] );
    }
  }

  return TETRIS_RET_OK;
}


bool piece_is_row_empty( PIECE_STRUCT_T *p_piece, uint8_t row_idx ){
  return _piece_is_segment_empty( p_piece, row_idx, true );
}

bool piece_is_col_empty( PIECE_STRUCT_T *p_piece, uint8_t col_idx ){
  return _piece_is_segment_empty( p_piece, col_idx, false );
}



/* ==========================================================================================================
 * Static Functions Declaration
 */


void _piece_copy_shape( PIECE_STRUCT_T *dst, piece_shape_t *src, uint8_t size ){
  for( uint8_t i=0; i<size; i++ ){
    dst->shape[i] = src[i];
  }
}


static inline bool _piece_is_segment_empty( PIECE_STRUCT_T *p_piece, uint8_t seg_idx, bool check_row ){
  uint8_t piece_base_idx = ( p_piece->order * seg_idx );
  uint8_t piece_idx      = 0;
  uint8_t i              = 0;

  if( check_row )
    piece_base_idx = ( p_piece->order * seg_idx );

  while( i < p_piece->order ){
    if( check_row ){
      piece_idx = piece_base_idx + i;
    }
    else{
      piece_idx = ( p_piece->order * i ) + seg_idx;
    }

    if( p_piece->shape[piece_idx] == 1 ){
      return false;
    }

    i++;
  }

  return true;
}