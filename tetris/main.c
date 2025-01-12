/*
 *  main.c
 *
 *  Created on: 11-Jan-2025
 *      Author: lucas-noce
 */

#include <stdlib.h>
#include <stdio.h>

#include "pieces.h"
#include "board.h"

int main( void ){
  printf( "board:\n" );
  board_init();
  // board_print();

  PIECE_STRUCT_T piece = { 0 };

  piece_get( PIECE_TYPE_L, &piece );
  
  printf( "original:\n" );
  piece_print( &piece );
  add_new_piece_to_board( &piece );

  printf( "\n\n" );
  board_print();

  for( uint8_t i=0; i<10; i++ ){
    if( move_piece_through_board( BOARD_DIRECTION_RIGHT, &piece ) != PIECE_RET_ERR ){
      printf( "\n\n" );
      board_print();
    }
  }

  return 0;
}
