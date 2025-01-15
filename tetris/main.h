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

#include "log_print.h"
#ifdef LOG_LEVEL
#undef LOG_LEVEL
#endif /* LOG_LEVEL */

/* ==========================================================================================================
 * Definitions
 */

#define LOG_LEVEL LOG_LEVEL_WRN

#define TETRIS_RET_OK   0
#define TETRIS_RET_ERR  1

/* ==========================================================================================================
 * Typedefs
 */

#endif /* _MAIN_H_ */