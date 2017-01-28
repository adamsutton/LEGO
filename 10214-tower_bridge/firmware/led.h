/* ****************************************************************************
 *
 * Copyright (C) 2017- Adam Sutton
 *
 * This file is part of ApsArduinoLib
 *
 * ApsArduinoLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ApsArduinoLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ApsArduinoLib.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For more details, including opportunities for alternative licensing,
 * please read the LICENSE file.
 *
 * ***************************************************************************
 *
 * LED control
 *
 * ***************************************************************************/

#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Initialise
 */
void led_init ( void );

/**
 * Enable / Disable LEDs
 *
 * @param leds The mask of LEDs to switch on
 */
void led_mask ( uint16_t leds );

/**
 * Turn ON specific LED
 */
void led_on ( uint8_t led );

/**
 * Turn OFF specific LED
 */
void led_off ( uint8_t led );

/**
 * Toggle specific LED
 */
void led_toggle ( uint8_t led );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LED_H__ */

/* ****************************************************************************
 * Editor Configuration
 *
 * vim:sts=2:ts=2:sw=2:et
 * ***************************************************************************/
