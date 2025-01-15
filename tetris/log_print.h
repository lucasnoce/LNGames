/*
 *  log_print.h
 *
 *  Created on: 14-Jan-2025
 *      Author: lucas-noce
 */

#ifndef _LOG_PRINT_H_
#define _LOG_PRINT_H_


#define LOG_LEVEL_NONE      0
#define LOG_LEVEL_BRD       1
#define LOG_LEVEL_INF       2
#define LOG_LEVEL_WRN       3
#define LOG_LEVEL_DBG       4
#define LOG_LEVEL_LAST_IDX  5


#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_NONE
#endif /* LOG_LEVEL */


#if ( LOG_LEVEL >= LOG_LEVEL_LAST_IDX )
#error "Invalid LOG_LEVEL"

#elif ( LOG_LEVEL == LOG_LEVEL_BRD )
#define LOG_BRD(...) printf(__VA_ARGS__)
#define LOG_INF(...) // Do nothing
#define LOG_WRN(...) // Do nothing
#define LOG_DBG(...) // Do nothing

#elif ( LOG_LEVEL == LOG_LEVEL_INF )
#define LOG_BRD(...) printf(__VA_ARGS__)
#define LOG_INF(...) printf("[inf] " __VA_ARGS__)
#define LOG_WRN(...) // Do nothing
#define LOG_DBG(...) // Do nothing

#elif ( LOG_LEVEL == LOG_LEVEL_WRN )
#define LOG_BRD(...) printf(__VA_ARGS__)
#define LOG_INF(...) printf("[inf] " __VA_ARGS__)
#define LOG_WRN(...) printf("[wrn] " __VA_ARGS__)
#define LOG_DBG(...) // Do nothing

#elif ( LOG_LEVEL == LOG_LEVEL_DBG )
#define LOG_BRD(...) printf(__VA_ARGS__)
#define LOG_INF(...) printf("[inf] " __VA_ARGS__)
#define LOG_WRN(...) printf("[wrn] " __VA_ARGS__)
#define LOG_DBG(...) printf("[dbg] " __VA_ARGS__)

#else
#define LOG_BRD(...) // Do nothing
#define LOG_INF(...) // Do nothing
#define LOG_WRN(...) // Do nothing
#define LOG_DBG(...) // Do nothing

#endif /* LOG_ */


#endif /* _LOG_PRINT_H_ */