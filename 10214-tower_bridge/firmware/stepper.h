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
 * Stepper motor API
 *
 * ***************************************************************************/

#ifndef __STEPPER_H__
#define __STEPPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Initialise
 */
void stepper_init          ( void );

/**
 * Run
 */
void stepper_run           ( void );


/**
 * Stop the motor
 */
void stepper_stop          ( void );

/**
 * Move the bridge
 *
 * @param dir   The direction -1 or 1 to move
 * @param rpm   The rate at which to move
 * @param steps The number of steps to move
 */
void stepper_move          ( int8_t dir, uint32_t rpm, uint32_t steps );

/**
 * Current direction
 */
int8_t stepper_get_dir    ( void );

/**
 * Current speed
 */
uint32_t stepper_get_rpm  ( void );

/**
 * Get the current "position" in steps
 */
uint32_t stepper_get_pos ( void );

/**
 * ZERO the stepper position
 */
void     stepper_zero_pos ( void );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __STEPPER_H__ */

/* ****************************************************************************
 * Editor Configuration
 *
 * vim:sts=2:ts=2:sw=2:et
 * ***************************************************************************/
