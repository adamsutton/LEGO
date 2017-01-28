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

#include "led.h"
#include "misc.h"
#include "log.h"

#include "Arduino.h"

/* LED pins */
static const uint8_t LED_PINS[] = {
  14, 15, 16, 17, 18, 19, 20, 21, 6
};

/* Current state */
uint16_t l_mask = 0x0;

/**
 * Initialise
 */
void
led_init ( void )
{
  uint8_t i;

  for (i = 0; i < ARRAY_SIZE(LED_PINS); i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
}

/*
 * Enable LEDs
 */
void
led_mask ( uint16_t mask )
{
  uint8_t i;

  debug("led: set mask %04X", mask);

  l_mask = mask;
  for (i = 0; i < ARRAY_SIZE(LED_PINS); i++) {
    digitalWrite(LED_PINS[i], (mask >> i) & 0x1);
  }
}

/*
 * Turn ON LED
 */
void
led_on ( uint8_t led )
{
  led_mask(l_mask | (0x1 << led));
}

/*
 * Turn OFF LED
 */
void
led_off ( uint8_t led )
{
  led_mask(l_mask & ~(0x1 << led));
}

/*
 * Toggle LED
 */
void
led_toggle ( uint8_t led )
{
  led_mask(l_mask ^ (0x1 << led));
}

/* ****************************************************************************
 * Editor Configuration
 *
 * vim:sts=2:ts=2:sw=2:et
 * ***************************************************************************/
