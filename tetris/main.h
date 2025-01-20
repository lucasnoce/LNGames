/*
 *  main.h
 *
 *  Created on: 11-Jan-2025
 *      Author: lucas-noce
 */

#ifndef _MAIN_H_
#define _MAIN_H_

/* ==========================================================================================================
 * Includes
 */

// #ifdef LOG_LEVEL
// #undef LOG_LEVEL
// #endif /* LOG_LEVEL */

/* ==========================================================================================================
 * Definitions
 */

#define LOG_LEVEL LOG_LEVEL_GAME

#define TETRIS_RET_OK            0
#define TETRIS_RET_ERR          -1
#define TETRIS_RET_ERR_NO_PIECE -2
#define TETRIS_RET_READY        -3  // ready for new piece

#define TETRIS_GAME_OVER     0
#define TETRIS_GAME_NOT_OVER 1
#define TETRIS_GAME_WON      2

#define TETRIS_GAME_INCREMENT_SPEED_DELAY_MS  5000

/* ==========================================================================================================
 * Includes
 */

#include "log_print.h"

/* ==========================================================================================================
 * Typedefs
 */

#endif /* _MAIN_H_ */