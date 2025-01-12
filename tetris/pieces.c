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

#define PIECE_SQUARE_MATRIX_ORDER     2
#define PIECE_T_MATRIX_ORDER          3
#define PIECE_LINE_MATRIX_ORDER       4
#define PIECE_Z_MATRIX_ORDER          3
#define PIECE_Z_FLIPPED_MATRIX_ORDER  3
#define PIECE_L_MATRIX_ORDER          3
#define PIECE_L_FLIPPED_MATRIX_ORDER  3

#define PIECE_SQUARE_SIZE     ( PIECE_SQUARE_MATRIX_ORDER * PIECE_SQUARE_MATRIX_ORDER )
#define PIECE_T_SIZE          ( PIECE_T_MATRIX_ORDER * PIECE_T_MATRIX_ORDER )
#define PIECE_LINE_SIZE       ( PIECE_LINE_MATRIX_ORDER * PIECE_LINE_MATRIX_ORDER )
#define PIECE_Z_SIZE          ( PIECE_Z_MATRIX_ORDER * PIECE_Z_MATRIX_ORDER )
#define PIECE_Z_FLIPPED_SIZE  ( PIECE_Z_FLIPPED_MATRIX_ORDER * PIECE_Z_FLIPPED_MATRIX_ORDER )
#define PIECE_L_SIZE          ( PIECE_L_MATRIX_ORDER * PIECE_L_MATRIX_ORDER )
#define PIECE_L_FLIPPED_SIZE  ( PIECE_L_FLIPPED_MATRIX_ORDER * PIECE_L_FLIPPED_MATRIX_ORDER )

#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

#define PIECE_LARGEST_MATRIX_ORDER  MAX(PIECE_SQUARE_MATRIX_ORDER, \
                                    MAX(PIECE_T_MATRIX_ORDER, \
                                    MAX(PIECE_LINE_MATRIX_ORDER, \
                                    MAX(PIECE_Z_MATRIX_ORDER, \
                                    MAX(PIECE_Z_FLIPPED_MATRIX_ORDER, \
                                    MAX(PIECE_L_MATRIX_ORDER, PIECE_L_FLIPPED_MATRIX_ORDER))))))

#define PIECE_LARGEST_SIZE          MAX(PIECE_SQUARE_SIZE, \
                                    MAX(PIECE_T_SIZE, \
                                    MAX(PIECE_LINE_SIZE, \
                                    MAX(PIECE_Z_SIZE, \
                                    MAX(PIECE_Z_FLIPPED_SIZE, \
                                    MAX(PIECE_L_SIZE, PIECE_L_FLIPPED_SIZE))))))


/* ==========================================================================================================
 * Static Typedefs
 */

/* ==========================================================================================================
 * Static variables
 */

static piece_format_t piece_square[PIECE_SQUARE_SIZE] = {
  1, 1,
  1, 1
};

static piece_format_t piece_T[PIECE_T_SIZE] = {
  0, 0, 0,
  0, 1, 0,
  1, 1, 1
};

static piece_format_t piece_line[PIECE_LINE_SIZE] = {
  0, 0, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0,
  1, 1, 1, 1
};

static piece_format_t piece_Z[PIECE_Z_SIZE] = {
  0, 0, 0,
  1, 1, 0,
  0, 1, 1
};

static piece_format_t piece_Z_flipped[PIECE_Z_FLIPPED_SIZE] = {
  0, 0, 0,
  0, 1, 1,
  1, 1, 0
};

static piece_format_t piece_L[PIECE_L_SIZE] = {
  0, 0, 0,
  0, 0, 1,
  1, 1, 1
};

static piece_format_t piece_L_flipped[PIECE_L_FLIPPED_SIZE] = {
  0, 0, 0,
  1, 0, 0,
  1, 1, 1
};


/* ==========================================================================================================
 * Static Function Prototypes
 */

/* ==========================================================================================================
 * Global Functions Declaration
 */

uint8_t piece_get( uint8_t type, PIECE_STRUCT_T *p_piece ){
  switch( type ){
    case PIECE_TYPE_SQUARE:
      p_piece->format = piece_square;
      p_piece->order  = PIECE_SQUARE_MATRIX_ORDER;
      p_piece->size   = PIECE_SQUARE_SIZE;
      break;

    case PIECE_TYPE_T:
      p_piece->format = piece_T;
      p_piece->order  = PIECE_T_MATRIX_ORDER;
      p_piece->size   = PIECE_T_SIZE;
      break;

    case PIECE_TYPE_LINE:
      p_piece->format = piece_line;
      p_piece->order  = PIECE_LINE_MATRIX_ORDER;
      p_piece->size   = PIECE_LINE_SIZE;
      break;

    case PIECE_TYPE_Z:
      p_piece->format = piece_Z;
      p_piece->order  = PIECE_Z_MATRIX_ORDER;
      p_piece->size   = PIECE_Z_SIZE;
      break;

    case PIECE_TYPE_Z_FLIPPED:
      p_piece->format = piece_Z_flipped;
      p_piece->order  = PIECE_Z_FLIPPED_MATRIX_ORDER;
      p_piece->size   = PIECE_Z_FLIPPED_SIZE;
      break;

    case PIECE_TYPE_L:
      p_piece->format = piece_L;
      p_piece->order  = PIECE_L_MATRIX_ORDER;
      p_piece->size   = PIECE_L_SIZE;
      break;

    case PIECE_TYPE_L_FLIPPED:
      p_piece->format = piece_L_flipped;
      p_piece->order  = PIECE_L_FLIPPED_MATRIX_ORDER;
      p_piece->size   = PIECE_L_FLIPPED_SIZE;
      break;

    case PIECE_TYPE_LAST_IDX:
    default:
      return TETRIS_RET_ERR;
  }

  return TETRIS_RET_OK;
}

void piece_rotate_90deg( PIECE_STRUCT_T *p_piece ){
  piece_format_t temp[PIECE_LARGEST_SIZE] = { 0 };
  uint8_t temp_idx = 0;
  uint8_t piece_idx = 0;

  for( uint8_t i=0; i<p_piece->order; i++ ){
    for( uint8_t j=0; j<p_piece->order; j++ ){
      piece_idx = (p_piece->order * i) + j;
      temp_idx  = (p_piece->order * j) + ( p_piece->order - 1 - i );
      temp[temp_idx] = p_piece->format[piece_idx];
    }
  }

  for( uint8_t i=0; i<p_piece->size; i++ ){
    p_piece->format[i] = temp[i];
  }
}

void piece_print( PIECE_STRUCT_T *p_piece ){
  uint8_t piece_idx = 0;

  for( uint8_t i=0; i<p_piece->order; i++ ){
    for( uint8_t j=0; j<p_piece->order; j++ ){
      piece_idx = (p_piece->order * i) + j;
      if( j == ( p_piece->order - 1 ) )
        printf( "%u\n", p_piece->format[piece_idx] );
      else
        printf( "%u, ", p_piece->format[piece_idx] );
    }
  }
}


/* ==========================================================================================================
 * Static Functions Declaration
 */
