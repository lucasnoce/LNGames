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
#include "game_config.h"
#include "graphics.h"
#include "board.h"
#include "score.h"


/* ==========================================================================================================
 * Definitions
 */



/* ==========================================================================================================
 * Static Typedefs
 */

/* ==========================================================================================================
 * Static variables
 */

static HANDLE h_game_reposition_mutex;
static HANDLE h_game_player_move_mutex;

static volatile uint32_t game_reposition_time  = GAME_CONFIG_BOARD_REPOSITION_MS;
static volatile uint32_t game_player_move_time = GAME_CONFIG_PLAYER_MOVE_DELAY_MS;

static const float game_reposition_speed_rate[GAME_DIFFICULTY_LAST_IDX] = {
  0.9, 0.75, 0.65, 0.5
};

/* ==========================================================================================================
 * Static Function Prototypes
 */

DWORD WINAPI _key_input_thread( void *data );
DWORD WINAPI _graphics_thread( void *data );
DWORD WINAPI _game_speed_thread( void *data );

static uint64_t _get_current_time_ms( void );
static void _flush_keyboard_buffer( void );


/* ==========================================================================================================
 * Global Functions Declaration
 */

int main_loop_init( void ){
  HANDLE threads[] = {
    CreateThread( NULL, 0, _key_input_thread, NULL, 0, NULL ),
    CreateThread( NULL, 0, _graphics_thread, NULL, 0, NULL ),
    CreateThread( NULL, 0, _game_speed_thread, NULL, 0, NULL )
  };

  HANDLE mutexes[] = {
    h_game_reposition_mutex,
    h_game_player_move_mutex
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

  uint8_t mutex_count = sizeof(mutexes) / 8;
  for( uint8_t i=0; i<mutex_count; i++ ){
    mutexes[i] = CreateMutex(NULL, FALSE, NULL);
    if( mutexes[i] == NULL ){
      LOG_DBG( "Failed to create h_game_reposition_mutex. Error: %lu\n", GetLastError() );
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
  for( uint8_t i=0; i<mutex_count; i++ ){
    CloseHandle(mutexes[i]);
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
      _flush_keyboard_buffer();
      LOG_INF( "You pressed: %c\n", key );

      switch( key ){
        case GAME_MOVE_DOWN_CHAR:
          move_current_piece_through_board( BOARD_DIRECTION_DOWN );
          graphics_print_game( false );
          break;

        case GAME_MOVE_LEFT_CHAR:
          move_current_piece_through_board( BOARD_DIRECTION_LEFT );
          graphics_print_game( false );
          break;

        case GAME_MOVE_RIGHT_CHAR:
          move_current_piece_through_board( BOARD_DIRECTION_RIGHT );
          graphics_print_game( false );
          break;

        case GAME_ROTATE_CLOCKWISE_CHAR:
          rotate_current_piece_through_board( true );
          graphics_print_game( false );
          break;

        case GAME_ROTATE_COUNTER_CLOCKWISE_CHAR:
          rotate_current_piece_through_board( false );
          graphics_print_game( false );
          break;

        case GAME_QUIT_CHAR:
          LOG_INF( "Quit\n" );
          return 1;

        default:
          Sleep( GAME_CONFIG_KEY_SAMPLE_RATE_MS );
          break;
      }

      WaitForSingleObject( h_game_player_move_mutex, INFINITE );
      Sleep( game_player_move_time );
      ReleaseMutex( h_game_player_move_mutex );
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
    
    if( graphics_print_game( true ) != TETRIS_RET_OK ){
      return 1;
    }

    LOG_DBG( "Graphics %u\n", i++ );

    WaitForSingleObject( h_game_reposition_mutex, INFINITE );

    current_time_ms = _get_current_time_ms();
    Sleep( game_reposition_time - ( current_time_ms - last_time_ms ) );

    ReleaseMutex( h_game_reposition_mutex );
  }

  return 0;
}


DWORD WINAPI _game_speed_thread( void *data ){
  while( 1 ){
    Sleep( TETRIS_GAME_INCREMENT_SPEED_DELAY_MS );
    
    WaitForSingleObject( h_game_reposition_mutex, INFINITE );
    game_reposition_time  = (uint32_t) ( (float) game_reposition_time * game_reposition_speed_rate[score_get_difficulty()] );
    ReleaseMutex( h_game_reposition_mutex );

    WaitForSingleObject( h_game_player_move_mutex, INFINITE );
    game_player_move_time = (uint32_t) ( (float) game_player_move_time * ( 2.0 - game_reposition_speed_rate[score_get_difficulty()] ) );
    ReleaseMutex( h_game_player_move_mutex );
  }
}


static uint64_t _get_current_time_ms( void ){
  struct timespec ts;
  clock_gettime( CLOCK_MONOTONIC, &ts ); // Use CLOCK_REALTIME if wall-clock time is needed
  return (uint64_t) ( ( ts.tv_sec ) * 1000 ) + ( ts.tv_nsec / 1000000 );
}


static void _flush_keyboard_buffer( void ){
  while (_kbhit()) {
    _getch();
  }
}