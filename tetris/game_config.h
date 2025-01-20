/*
 *  game_config.h
 *
 *  Created on: 16-Jan-2025
 *      Author: lucas-noce
 */

#include <stdint.h>

#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_


#define GAME_MOVE_DOWN_CHAR   's'
#define GAME_MOVE_LEFT_CHAR   'a'
#define GAME_MOVE_RIGHT_CHAR  'd'
#define GAME_ROTATE_CHAR      'r'
#define GAME_QUIT_CHAR        'q'

#define GAME_CONFIG_KEY_SAMPLE_RATE_MS    10
#define GAME_CONFIG_BOARD_REPOSITION_MS   ( (uint64_t) 1000 )
#define GAME_CONFIG_PLAYER_MOVE_DELAY_MS  250

#define GAME_CONFIG_PRINT_BOARD_V_BORDER_CHAR         '|'
#define GAME_CONFIG_PRINT_BOARD_H_BORDER_CHAR         '-'
#define GAME_CONFIG_PRINT_BOARD_ROW_SEP_CHAR          ' '
#define GAME_CONFIG_PRINT_BOARD_COL_SEP_CHAR          '.'
#define GAME_CONFIG_PRINT_BOARD_PIECE_SQUARE_CHAR     '#'
#define GAME_CONFIG_PRINT_BOARD_PIECE_T_CHAR          'T'
#define GAME_CONFIG_PRINT_BOARD_PIECE_LINE_CHAR       'I'
#define GAME_CONFIG_PRINT_BOARD_PIECE_Z_CHAR          'Z'
#define GAME_CONFIG_PRINT_BOARD_PIECE_Z_FLIPPED_CHAR  'S'
#define GAME_CONFIG_PRINT_BOARD_PIECE_L_CHAR          'L'
#define GAME_CONFIG_PRINT_BOARD_PIECE_L_FLIPPED_CHAR  '1'









#endif /* _GAME_CONFIG_H_ */