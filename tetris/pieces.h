/*
 *  pieces.h
 *
 *  Created on: 11-Jan-2025
 *      Author: lucas-noce
 */

#ifndef _PIECES_H_
#define _PIECES_H_


/* ==========================================================================================================
 * Includes
 */

#include <stdint.h>


/* ==========================================================================================================
 * Definitions
 */

#define PIECE_RET_OK  0
#define PIECE_RET_ERR 1


/* ==========================================================================================================
 * Typedefs
 */

typedef enum{
  PIECE_TYPE_SQUARE = 0,
  PIECE_TYPE_T,
  PIECE_TYPE_LINE,
  PIECE_TYPE_Z,
  PIECE_TYPE_Z_FLIPPED,
  PIECE_TYPE_L,
  PIECE_TYPE_L_FLIPPED,
  PIECE_TYPE_LAST_IDX,
} PIECE_TYPES_E;

typedef uint8_t piece_format_t;

typedef struct PIECE_STRUCT_TAG{
  uint8_t order;            // order (N) of the square matrix (N x N)
  uint8_t size;             // number of elements in the matrix
  uint8_t position_row;     // indexed on top left corner of the piece
  uint8_t position_col;     // indexed on top left corner of the piece
  uint8_t displayed_rows;   // number of rows displeyd in the board (starts with 0 and goes up to 'order')
  piece_format_t *format;   // pointer to the "matrix" of the piece
} PIECE_STRUCT_T;


/* ==========================================================================================================
 * Global Functions
 */

uint8_t piece_get( uint8_t type, PIECE_STRUCT_T *p_piece );
void piece_rotate_90deg( PIECE_STRUCT_T *p_piece );
void piece_print( PIECE_STRUCT_T *p_piece );


#endif /* _PIECES_H_ */