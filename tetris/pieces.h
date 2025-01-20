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
#include <stdbool.h>


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
 * Typedefs
 */

/*!
  @brief        Indicates the possible types/shape of pieces.
*/
typedef enum{
  PIECE_SHAPE_SQUARE = 0,
  PIECE_SHAPE_T,
  PIECE_SHAPE_LINE,
  PIECE_SHAPE_Z,
  PIECE_SHAPE_Z_FLIPPED,
  PIECE_SHAPE_L,
  PIECE_SHAPE_L_FLIPPED,
  PIECE_SHAPE_LAST_IDX,
} PIECE_SHAPES_E;

/*!
  @brief        Wrapper type used to indicate a piece vector.
*/
typedef uint8_t piece_shape_t;

/*!
  @brief        Indicates all the piece parameters.

  @param        order: the order (n) of the square matrix (n x n) that describes the piece shape.
  @param        size: the total number of elements in the square matrix (size = order^2).
  @param        displayed_rows: number of piece rows displeyd in the board (starts with 0 and goes up to 'order').
  @param        displayed_cols: number of piece cols displeyd in the board (starts with 0 and goes up to 'order').
  @param        position_row: the row number of the board at which the piece starts.
  @param        position_col: the column number of the board at which the piece starts.
  @param        is_colliding: flag to identify whether the piece is currently colliding at the downwards direction.
  @param        is_moving: flag to identify whether the piece is still allowed to move through the board.
  @param        shape: pointer to the square matrix that describes the piece's shape.

  @warning      Beware of `position_row` and `position_col` being signed integers to account for pieces being
                positioned all the way up or to the left with negative indexes.

  @note         Despite being used as a matrix, a piece is actually described as a single dimension array.
*/
typedef struct PIECE_STRUCT_TAG{
  uint8_t order;
  uint8_t size;
  uint8_t displayed_rows;
  uint8_t displayed_cols;
  int8_t  position_row;
  int8_t  position_col;
  char print_char;
  bool is_colliding;
  bool is_moving;
  piece_shape_t shape[PIECE_LARGEST_SIZE];
} PIECE_STRUCT_T;


/* ==========================================================================================================
 * Global Functions
 */

/*!
  @brief        Retrieves a piece of a certain type, used to generate new pieces.

  @param[in]    type: one of the piece shape types (from PIECE_SHAPES_E).
  @param[out]   p_piece: pointer to the retrieved piece.

  @returns      One of the possible TETRIS_RET_x macro values (defined in main.h).
*/
int8_t piece_get( uint8_t type, PIECE_STRUCT_T *p_piece );

/*!
  @brief        Rotate a piece 90 degrees clockwise around its center.

  @param[in]    p_piece: pointer to the piece to be rotated.

  @returns      One of the possible TETRIS_RET_x macro values (defined in main.h).
*/
int8_t piece_rotate_90deg( PIECE_STRUCT_T *p_piece );

/*!
  @brief        Prints a piece in its current orientation.

  @param[in]    p_piece: pointer to the piece to be printed.

  @returns      One of the possible TETRIS_RET_x macro values (defined in main.h).
*/
int8_t piece_print( PIECE_STRUCT_T *p_piece );

/*!
  @brief        Checks if a row of the piece is completely empty, i.e. every element is 0.

  @param[in]    p_piece: pointer to the piece to be checked.
  @param[in]    row_idx: the row to be checked.

  @returns      true if row is empty, false otherwise.
*/
bool piece_is_row_empty( PIECE_STRUCT_T *p_piece, uint8_t row_idx );

/*!
  @brief        Checks if a col of the piece is completely empty, i.e. every element is 0.

  @param[in]    p_piece: pointer to the piece to be checked.
  @param[in]    col_idx: the col to be checked.

  @returns      true if col is empty, false otherwise.
*/
bool piece_is_col_empty( PIECE_STRUCT_T *p_piece, uint8_t col_idx );

/*!
  @brief        Checks if a piece is new, i.e. can still move through the board.

  @param[in]    p_piece: pointer to the piece to be checked.

  @returns      true if piece is new, false otherwise.
*/
bool piece_is_moving( PIECE_STRUCT_T *p_piece );

#endif /* _PIECES_H_ */