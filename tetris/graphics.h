/*
 *  graphics.c
 *
 *  Created on: 16-Jan-2025
 *      Author: lucas-noce
 */

#include <stdlib.h>
#include <stdint.h>


#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


uint8_t graphics_init( void );
void graphics_deinit( void );
void graphics_clear_screen( void );
uint8_t graphics_print_game( bool try_fix );

#endif /* _GRAPHICS_H_ */