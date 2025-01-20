/*
 *  game_config.h
 *
 *  Created on: 16-Jan-2025
 *      Author: lucas-noce
 */

#include <stdint.h>

#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_


/*!
  @brief        Indicates the possible colors of a piece in the board.
*/
#define GAME_PIECE_COLOR_RESET    0
#define GAME_PIECE_COLOR_MAGENTA  1
#define GAME_PIECE_COLOR_RED      2
#define GAME_PIECE_COLOR_YELLOW   3
#define GAME_PIECE_COLOR_GREEN    4
#define GAME_PIECE_COLOR_CYAN     5
#define GAME_PIECE_COLOR_BLUE     6
#define GAME_PIECE_COLOR_COUNT    7


#define GAME_MOVE_DOWN_CHAR   's'
#define GAME_MOVE_LEFT_CHAR   'a'
#define GAME_MOVE_RIGHT_CHAR  'd'
#define GAME_ROTATE_CHAR      'r'
#define GAME_QUIT_CHAR        'q'

#define GAME_CONFIG_KEY_SAMPLE_RATE_MS    10
#define GAME_CONFIG_BOARD_REPOSITION_MS   ( (uint64_t) 800 )
#define GAME_CONFIG_PLAYER_MOVE_DELAY_MS  250

#define GAME_CONFIG_PRINT_BOARD_V_BORDER_CHAR         '|'
#define GAME_CONFIG_PRINT_BOARD_H_BORDER_CHAR         '_'
#define GAME_CONFIG_PRINT_BOARD_ROW_SEP_CHAR          '_'
#define GAME_CONFIG_PRINT_BOARD_COL_SEP_CHAR          '|'

#define GAME_CONFIG_PRINT_BOARD_PIECE_SQUARE_CHAR     '#'
#define GAME_CONFIG_PRINT_BOARD_PIECE_T_CHAR          'T'
#define GAME_CONFIG_PRINT_BOARD_PIECE_LINE_CHAR       'I'
#define GAME_CONFIG_PRINT_BOARD_PIECE_Z_CHAR          'Z'
#define GAME_CONFIG_PRINT_BOARD_PIECE_Z_FLIPPED_CHAR  'S'
#define GAME_CONFIG_PRINT_BOARD_PIECE_L_CHAR          'L'
#define GAME_CONFIG_PRINT_BOARD_PIECE_L_FLIPPED_CHAR  '1'

#define GAME_CONFIG_PRINT_BOARD_PIECE_SQUARE_COLOR     GAME_PIECE_COLOR_YELLOW
#define GAME_CONFIG_PRINT_BOARD_PIECE_T_COLOR          GAME_PIECE_COLOR_RED
#define GAME_CONFIG_PRINT_BOARD_PIECE_LINE_COLOR       GAME_PIECE_COLOR_CYAN
#define GAME_CONFIG_PRINT_BOARD_PIECE_Z_COLOR          GAME_PIECE_COLOR_GREEN
#define GAME_CONFIG_PRINT_BOARD_PIECE_Z_FLIPPED_COLOR  GAME_PIECE_COLOR_MAGENTA
#define GAME_CONFIG_PRINT_BOARD_PIECE_L_COLOR          GAME_PIECE_COLOR_BLUE
#define GAME_CONFIG_PRINT_BOARD_PIECE_L_FLIPPED_COLOR  GAME_PIECE_COLOR_YELLOW


#endif /* _GAME_CONFIG_H_ */