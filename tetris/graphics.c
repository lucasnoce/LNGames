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
#include <windows.h>

#include "board.h"

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

void graphics_print_game( void ){
  board_print();
}