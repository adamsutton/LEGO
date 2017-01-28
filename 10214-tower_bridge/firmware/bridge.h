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
 * Bridge control
 *
 * ***************************************************************************/

#ifndef __BRIDGE_H__
#define __BRIDGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Initialise
 */
void bridge_init ( void );

/**
 * Run
 */
void bridge_run  ( void );

/**
 * Up
 *
 * @param rpm Speed to move (0 = default)
 */
void bridge_up   ( uint32_t rpm );

/**
 * Down
 *
 * @param rpm Speed to move (0 = default)
 */
void bridge_down ( uint32_t rpm );

/**
 * Position the bridge at specific point
 *
 * Note: Position is measured as specific number of steps from DOWN
 *
 * @param pos The number of steps from DOWN
 * @param rpm Speed to move (0 = default)
 */
void bridge_position ( uint32_t pos, uint32_t rpm );

/**
 * Stop
 */
void bridge_stop ( void );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __BRIDGE_H__ */

/* ****************************************************************************
 * Editor Configuration
 *
 * vim:sts=2:ts=2:sw=2:et
 * ***************************************************************************/
