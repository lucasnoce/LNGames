/*
 *  main.c
 *
 *  Created on: 11-Jan-2025
 *      Author: lucas-noce
 */

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "main.h"
#include "pieces.h"
#include "board.h"
#include "main_loop.h"

void test_function( void );

int main( void ){
  main_loop_init();
  // test_function();

  return 0;
}

void test_function( void ){
  LOG_DBG( "board:\n" );
  board_init();
  // board_print();

  PIECE_STRUCT_T piece = { 0 };
  uint8_t piece_type = PIECE_SHAPE_L;

  piece_get( piece_type, &piece );
  
  LOG_DBG( "original:\n" );
  piece_print( &piece );
  LOG_DBG( "\n" );
  piece_rotate_90deg( &piece );
  piece_rotate_90deg( &piece );
  piece_rotate_90deg( &piece );

  add_new_piece_to_board( piece_type );

  LOG_DBG( "\n" );
  board_print();
  LOG_DBG( "\n\n" );

  for( uint8_t i=0; i<5; i++ ){
      // board_print();
    if( move_current_piece_through_board( BOARD_DIRECTION_DOWN ) != TETRIS_RET_ERR ){
      board_print();
      LOG_DBG( "\n\n" );
    }
  }
}