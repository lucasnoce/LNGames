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


/* ==========================================================================================================
 * Definitions
 */

#define GAME_QUIT_CHAR  'q'

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


/* ==========================================================================================================
 * Global Functions Declaration
 */

int main_loop_init( void ){
  HANDLE thread[1];
  thread[0] = CreateThread( NULL, 0, _key_input_thread, NULL, 0, NULL );

  if( thread[0] == NULL ){
    printf( "Thread creation error\n" );
    return 1;
  }

  WaitForMultipleObjects( 1, thread, TRUE, INFINITE );

  return 0;
}


/* ==========================================================================================================
 * Static Functions Declaration
 */

DWORD WINAPI _key_input_thread( void *data ){
  char key;

  while( 1 ){
    if( _kbhit() ) {  // Check if a key is pressed
      key = _getch();  // Read the key
      LOG_INF("You pressed: %c\n", key);

      if( key == GAME_QUIT_CHAR ){
        printf("Quit\n");
        return 1;
      }
    }

    Sleep(1);
  }

  return 0;
}
