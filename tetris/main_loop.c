/*
 *  main_loop.c
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
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

#include "main.h"
#include "graphics.h"


/* ==========================================================================================================
 * Definitions
 */

#define GAME_MOVE_DOWN_CHAR   's'
#define GAME_MOVE_LEFT_CHAR   'a'
#define GAME_MOVE_RIGHT_CHAR  'd'
#define GAME_QUIT_CHAR        'q'

/* ==========================================================================================================
 * Static Typedefs
 */

/* ==========================================================================================================
 * Static variables
 */

/* ==========================================================================================================
 * Static Function Prototypes
 */

DWORD WINAPI _key_input_thread( void *data );
DWORD WINAPI _graphics_thread( void *data );


/* ==========================================================================================================
 * Global Functions Declaration
 */

int main_loop_init( void ){
  HANDLE threads[] = {
    CreateThread( NULL, 0, _key_input_thread, NULL, 0, NULL ),
    CreateThread( NULL, 0, _graphics_thread, NULL, 0, NULL )
  };

  LOG_DBG("size: %lld\n", sizeof(threads)/8);

  uint8_t thread_count = sizeof(threads) / 8;
  for( uint8_t i=0; i<thread_count; i++ ){
    if( threads[i] == NULL ){
      LOG_DBG( "Thread creation error: %u\n", i );
      return 1;
    }
  }

  DWORD finished_thread = WaitForMultipleObjects( thread_count, threads, FALSE, INFINITE );
  uint8_t finished_idx  = finished_thread - WAIT_OBJECT_0;

  if( finished_idx >= 0 && finished_idx < thread_count ){
    DWORD exitCode;
    GetExitCodeThread( threads[finished_idx], &exitCode );

    LOG_DBG( "Thread %d finished with return value: %lu\n", finished_idx, exitCode );

    for( uint8_t i=0; i<thread_count; i++ ){
      if( i == finished_idx ) continue;

      TerminateThread( threads[i], 0 );

      LOG_DBG( "Thread %d terminated.\n", i );
    }
  }

  for( uint8_t i=0; i<thread_count; i++ ){
    CloseHandle(threads[i]);
  }

  return 0;
}


/* ==========================================================================================================
 * Static Functions Declaration
 */

DWORD WINAPI _key_input_thread( void *data ){
  char key;

  while( 1 ){
    if( _kbhit() ) {
      key = _getch();
      LOG_DBG( "You pressed: %c\n", key );

      switch( key ){
        case GAME_MOVE_DOWN_CHAR:
          break;

        case GAME_MOVE_LEFT_CHAR:
          break;

        case GAME_MOVE_RIGHT_CHAR:
          break;

        case GAME_QUIT_CHAR:
          LOG_INF( "Quit\n" );
          return 1;

        default:
          break;
      }
    }

    Sleep( 1 );
  }

  return 0;
}

DWORD WINAPI _graphics_thread( void *data ){
  uint16_t i = 0;
  graphics_init();
  
  while( 1 ){
    graphics_clear_screen();
    graphics_print_game();
    LOG_DBG( "Graphics %u\n", i++ );
    Sleep( 1000 );
  }

  return 0;
}