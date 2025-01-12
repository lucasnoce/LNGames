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

#include "main.h"
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


/* ==========================================================================================================
 * Global Functions Declaration
 */


uint8_t piece_get( uint8_t type, PIECE_STRUCT_T *p_piece ){
  switch( type ){
    case PIECE_SHAPE_SQUARE:
      p_piece->order = PIECE_SQUARE_MATRIX_ORDER;
      p_piece->size  = PIECE_SQUARE_SIZE;
      _piece_copy_shape( p_piece, piece_square, p_piece->size );
      break;

    case PIECE_SHAPE_T:
      p_piece->order = PIECE_T_MATRIX_ORDER;
      p_piece->size  = PIECE_T_SIZE;
      _piece_copy_shape( p_piece, piece_T, p_piece->size );
      break;

    case PIECE_SHAPE_LINE:
      p_piece->order = PIECE_LINE_MATRIX_ORDER;
      p_piece->size  = PIECE_LINE_SIZE;
      _piece_copy_shape( p_piece, piece_line, p_piece->size );
      break;

    case PIECE_SHAPE_Z:
      p_piece->order = PIECE_Z_MATRIX_ORDER;
      p_piece->size  = PIECE_Z_SIZE;
      _piece_copy_shape( p_piece, piece_Z, p_piece->size );
      break;

    case PIECE_SHAPE_Z_FLIPPED:
      p_piece->order = PIECE_Z_FLIPPED_MATRIX_ORDER;
      p_piece->size  = PIECE_Z_FLIPPED_SIZE;
      _piece_copy_shape( p_piece, piece_Z_flipped, p_piece->size );
      break;

    case PIECE_SHAPE_L:
      p_piece->order = PIECE_L_MATRIX_ORDER;
      p_piece->size  = PIECE_L_SIZE;
      _piece_copy_shape( p_piece, piece_L, p_piece->size );
      break;

    case PIECE_SHAPE_L_FLIPPED:
      p_piece->order = PIECE_L_FLIPPED_MATRIX_ORDER;
      p_piece->size  = PIECE_L_FLIPPED_SIZE;
      _piece_copy_shape( p_piece, piece_L_flipped, p_piece->size );
      break;

    case PIECE_SHAPE_LAST_IDX:
    default:
      return TETRIS_RET_ERR;
  }

  return TETRIS_RET_OK;
}


void piece_rotate_90deg( PIECE_STRUCT_T *p_piece ){
  piece_shape_t temp[PIECE_LARGEST_SIZE] = { 0 };
  uint8_t temp_idx = 0;
  uint8_t piece_idx = 0;

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
}


void piece_print( PIECE_STRUCT_T *p_piece ){
  uint8_t piece_idx = 0;

  for( uint8_t i=0; i<p_piece->order; i++ ){
    for( uint8_t j=0; j<p_piece->order; j++ ){
      piece_idx = (p_piece->order * i) + j;
      if( j == ( p_piece->order - 1 ) )
        printf( "%u\n", p_piece->shape[piece_idx] );
      else
        printf( "%u, ", p_piece->shape[piece_idx] );
    }
  }
}


/* ==========================================================================================================
 * Static Functions Declaration
 */


void _piece_copy_shape( PIECE_STRUCT_T *dst, piece_shape_t *src, uint8_t size ){
  for( uint8_t i=0; i<size; i++ ){
    dst->shape[i] = src[i];
  }
}