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
  LOG_INF( "board:\n" );
  board_init();
  // board_print();

  PIECE_STRUCT_T piece = { 0 };

  piece_get( PIECE_SHAPE_L, &piece );
  
  LOG_INF( "original:\n" );
  piece_print( &piece );
  LOG_INF( "\n" );
  LOF_INF( "row 2 empty: %u\n", piece_is_row_empty( &piece, 2 ) );
  LOF_INF( "row 1 empty: %u\n", piece_is_row_empty( &piece, 1 ) );
  LOF_INF( "row 0 empty: %u\n", piece_is_row_empty( &piece, 0 ) );
  
  piece_rotate_90deg( &piece );
  piece_rotate_90deg( &piece );

  LOF_INF( "row 2 empty: %u\n", piece_is_row_empty( &piece, 2 ) );
  LOF_INF( "row 1 empty: %u\n", piece_is_row_empty( &piece, 1 ) );
  LOF_INF( "row 0 empty: %u\n", piece_is_row_empty( &piece, 0 ) );


  // add_new_piece_to_board( &piece );

  // // LOG_INF( "\n" );
  // // board_print();
  // // LOG_INF( "\n\n" );

  // // for( uint8_t i=0; i<6; i++ ){
  // //     // board_print();
  // //   if( move_piece_through_board( BOARD_DIRECTION_DOWN, &piece ) != TETRIS_RET_ERR ){
  // //     board_print();
  // //     LOG_INF( "\n\n" );
  // //   }
  // //   else{
  // //     LOG_INF( "errooo\n" );
  // //   }
  // // }

  return 0;
}


/*!
  @brief        
  @param[in]    
  @param[out]   
  @returns      
  @note         
*/