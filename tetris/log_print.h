/*
 *  log_print.h
 *
 *  Created on: 14-Jan-2025
 *      Author: lucas-noce
 */

#ifndef _LOG_PRINT_H_
#define _LOG_PRINT_H_

#define LOG_LEVEL_NONE  0
#define LOG_LEVEL_INF   1
#define LOG_LEVEL_DBG   2

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_NONE
#endif /* LOG_LEVEL */

#if ( LOG_LEVEL >= LOG_LEVEL_DBG )
#define LOG_INF(...) printf(__VA_ARGS__)
#define LOG_DBG(...) printf(__VA_ARGS__)

#elif ( LOG_LEVEL == LOG_LEVEL_INF )
#define LOG_INF(...) printf(__VA_ARGS__)
#define LOG_DBG(...) // Do nothing

#else
#define LOG_INF(...) // Do nothing
#define LOG_DBG(...) // Do nothing

#endif /* LOG_DBG */

#endif /* _LOG_PRINT_H_ */