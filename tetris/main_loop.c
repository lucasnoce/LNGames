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
#include <time.h>
#include <conio.h>
#include <windows.h>

#include "main.h"
#include "graphics.h"
#include "board.h"


/* ==========================================================================================================
 * Definitions
 */

#define GAME_MOVE_DOWN_CHAR   's'
#define GAME_MOVE_LEFT_CHAR   'a'
#define GAME_MOVE_RIGHT_CHAR  'd'
#define GAME_ROTATE_CHAR      'r'
#define GAME_QUIT_CHAR        'q'

#define GAME_CONFIG_KEY_SAMPLE_RATE_MS    10
#define GAME_CONFIG_BOARD_REPOSITION_MS   ( (uint64_t) 1000 )
#define GAME_CONFIG_PLAYER_MOVE_DELAY_MS  250

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
static uint64_t _get_current_time_ms( void );


/* ==========================================================================================================
 * Global Functions Declaration
 */

int main_loop_init( void ){
  HANDLE threads[] = {
    CreateThread( NULL, 0, _key_input_thread, NULL, 0, NULL ),
    CreateThread( NULL, 0, _graphics_thread, NULL, 0, NULL )
  };

  uint8_t ret = graphics_init();
  if( ret != 0 )
    return 1;

  LOG_INF("size: %lld\n", sizeof(threads)/8);

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

  graphics_deinit();
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
      LOG_INF( "You pressed: %c\n", key );

      switch( key ){
        case GAME_MOVE_DOWN_CHAR:
          move_current_piece_through_board( BOARD_DIRECTION_DOWN );
          graphics_print_game( false );
          Sleep( GAME_CONFIG_PLAYER_MOVE_DELAY_MS );
          break;

        case GAME_MOVE_LEFT_CHAR:
          move_current_piece_through_board( BOARD_DIRECTION_LEFT );
          graphics_print_game( false );
          Sleep( GAME_CONFIG_PLAYER_MOVE_DELAY_MS );
          break;

        case GAME_MOVE_RIGHT_CHAR:
          move_current_piece_through_board( BOARD_DIRECTION_RIGHT );
          graphics_print_game( false );
          Sleep( GAME_CONFIG_PLAYER_MOVE_DELAY_MS );
          break;

        case GAME_ROTATE_CHAR:
          rotate_current_piece_through_board();
          graphics_print_game( false );
          Sleep( GAME_CONFIG_PLAYER_MOVE_DELAY_MS );
          break;

        case GAME_QUIT_CHAR:
          LOG_INF( "Quit\n" );
          return 1;

        default:
          Sleep( GAME_CONFIG_KEY_SAMPLE_RATE_MS );
          break;
      }
    }
  }

  return 0;
}

DWORD WINAPI _graphics_thread( void *data ){
  uint64_t last_time_ms    = 0;
  uint64_t current_time_ms = 0;
  uint16_t i = 0;
  
  while( 1 ){
    last_time_ms = _get_current_time_ms();
    
    // graphics_clear_screen();

    if( graphics_print_game( true ) != TETRIS_RET_OK ){
      return 1;
    }

    // graphics_print_score();
    
    LOG_DBG( "Graphics %u\n", i++ );
    current_time_ms = _get_current_time_ms();
    Sleep( GAME_CONFIG_BOARD_REPOSITION_MS - ( current_time_ms - last_time_ms ) );
  }

  return 0;
}


static uint64_t _get_current_time_ms( void ){
  struct timespec ts;
  clock_gettime( CLOCK_MONOTONIC, &ts ); // Use CLOCK_REALTIME if wall-clock time is needed
  return (uint64_t) ( ( ts.tv_sec ) * 1000 ) + ( ts.tv_nsec / 1000000 );
}