/*
 *  main.c
 *
 *  Created on: 11-Jan-2025
 *      Author: lucas-noce
 */

#include <stdlib.h>
#include <stdio.h>

#include "main.h"
#include "pieces.h"
#include "board.h"

int main( void ){
  printf( "board:\n" );
  board_init();
  // board_print();

  PIECE_STRUCT_T piece = { 0 };

  piece_get( PIECE_TYPE_Z, &piece );
  
  // printf( "original:\n" );
  // piece_print( &piece );
  add_new_piece_to_board( &piece );

  // printf( "\n" );
  board_print();
  printf( "\n\n" );

  for( uint8_t i=0; i<6; i++ ){
      // board_print();
    if( move_piece_through_board( BOARD_DIRECTION_LEFT, &piece ) != TETRIS_RET_ERR ){
      board_print();
      printf( "\n\n" );
    }
  }

  return 0;
}
