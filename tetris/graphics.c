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


static HANDLE hMutex;

static char *game_over_text[] = {
"  _______      ___      .___  ___.  _______          ",
" /  _____|    /   \\     |   \\/   | |   ____|         ",
"|  |  __     /  ^  \\    |  \\  /  | |  |__            ",
"|  | |_ |   /  /_\\  \\   |  |\\/|  | |   __|           ",
"|  |__| |  /  _____  \\  |  |  |  | |  |____          ",
" \\______| /__/     \\__\\ |__|  |__| |_______|         ",
"                                                     ",
"  ______   ____    ____  _______ .______          __ ",
" /  __  \\  \\   \\  /   / |   ____||   _  \\        |  |",
"|  |  |  |  \\   \\/   /  |  |__   |  |_)  |       |  |",
"|  |  |  |   \\      /   |   __|  |      /        |  |",
"|  `--'  |    \\    /    |  |____ |  |\\  \\----.   |__|",
" \\______/      \\__/     |_______||__| `._____|   (__)"
};

static char *you_win_text[] = {
"____    ____  ______    __    __         ",
"\\   \\  /   / /  __  \\  |  |  |  |        ",
" \\   \\/   / |  |  |  | |  |  |  |        ",
"  \\_    _/  |  |  |  | |  |  |  |        ",
"    |  |    |  `--'  | |  `--'  |        ",
"    |__|     \\______/   \\______/         ",
"                                         ",
"____    __    ____  __  .__   __.     __ ",
"\\   \\  /  \\  /   / |  | |  \\ |  |    |  |",
" \\   \\/    \\/   /  |  | |   \\|  |    |  |",
"  \\            /   |  | |  . `  |    |  |",
"   \\    /\\    /    |  | |  |\\   |    |__|",
"    \\__/  \\__/     |__| |__| \\__|    (__)"
};


static void _graphics_print_game_over( void );
static void _graphics_print_you_win( void );


uint8_t graphics_init( void ){
  hMutex = CreateMutex(NULL, FALSE, NULL);

  if( hMutex == NULL ){
    printf("Failed to create mutex. Error: %lu\n", GetLastError());
    return 1;
  }
  
  board_init();

  return 0;
}

void graphics_deinit( void ){
  CloseHandle( hMutex );
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
  WaitForSingleObject(hMutex, INFINITE);

  graphics_clear_screen();

  if( fix_current_piece_on_board() != TETRIS_RET_OK ){
    uint8_t new_piece_type = 0;

    if( check_complete_row() == TETRIS_GAME_OVER ){
      _graphics_print_game_over();
      return -TETRIS_RET_ERR;
    }

    LOG_INF( "fix piece\n" );

    srand( time( NULL ) );
    new_piece_type = rand() % PIECE_SHAPE_LAST_IDX;
    
    add_new_piece_to_board( new_piece_type );
  }

  move_current_piece_through_board( BOARD_DIRECTION_DOWN );
  board_print();

  ReleaseMutex(hMutex);
  return TETRIS_RET_OK;
}

static void _graphics_print_game_over( void ){
  LOG_GAME( "\n\n" );
  for( uint8_t i=0; i<(sizeof(game_over_text)/sizeof(game_over_text[0])); i++ ){
    LOG_GAME( "%s\n", game_over_text[i] );
  }
  LOG_GAME( "\n\n" );
}

static void _graphics_print_you_win( void ){
  LOG_GAME( "\n\n" );
  for( uint8_t i=0; i<(sizeof(you_win_text)/sizeof(you_win_text[0])); i++ ){
    LOG_GAME( "%s\n", you_win_text[i] );
  }
  LOG_GAME( "\n\n" );
}