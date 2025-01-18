/*
 *  graphics.c
 *
 *  Created on: 16-Jan-2025
 *      Author: lucas-noce
 */

/* ==========================================================================================================
 * Includes
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>

#include "main.h"
#include "board.h"
#include "pieces.h"


void graphics_init( void ){
  board_init();
}


void graphics_clear_screen( void ){
  HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD written;
  DWORD consoleSize;

  GetConsoleScreenBufferInfo( hConsole, &csbi );
  consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

  FillConsoleOutputCharacter( hConsole, ' ', consoleSize, (COORD){0, 0}, &written );

  SetConsoleCursorPosition( hConsole, (COORD){0, 0} );
}


uint8_t graphics_print_game( void ){
  if( fix_current_piece_on_board() != TETRIS_RET_OK ){
    uint8_t new_piece_type = 0;

    if( check_complete_row() == TETRIS_GAME_OVER ){
      LOG_GAME( "GAME OVER\n" );
      return -TETRIS_RET_ERR;
    }

    LOG_INF( "fix piece\n" );

    srand( time( NULL ) );
    new_piece_type = rand() % PIECE_SHAPE_LAST_IDX;
    
    add_new_piece_to_board( new_piece_type );
  }

  move_current_piece_through_board( BOARD_DIRECTION_DOWN );
  board_print();
  return TETRIS_RET_OK;
}