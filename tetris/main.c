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
  LOG_INF( "board:\n" );
  board_init();
  // board_print();

  PIECE_STRUCT_T piece = { 0 };

  piece_get( PIECE_SHAPE_L, &piece );
  
  LOG_INF( "original:\n" );
  piece_print( &piece );
  LOG_INF( "\n" );
  piece_rotate_90deg( &piece );
  piece_rotate_90deg( &piece );
  piece_rotate_90deg( &piece );

  add_new_piece_to_board( &piece );

  LOG_INF( "\n" );
  board_print();
  LOG_INF( "\n\n" );

  for( uint8_t i=0; i<5; i++ ){
      // board_print();
    if( move_piece_through_board( BOARD_DIRECTION_DOWN, &piece ) != TETRIS_RET_ERR ){
      board_print();
      LOG_INF( "\n\n" );
    }
  }
}