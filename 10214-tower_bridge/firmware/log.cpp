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
 * Logging
 *
 * ***************************************************************************/

#include "log.h"

#include "Arduino.h"
#include "HardwareSerial.h"

#include <stdio.h>
#include <stdarg.h>

static char l_buf[256];
static uint8_t l_pin = 0, l_pout = 0;

/*
 * Log it
 */
void
debug ( const char *fmt, ... )
{
  size_t n;
  va_list args;
  static char buf[128];
  uint8_t pin, i;

  va_start(args, fmt);
  pin = l_pin;
  n   =  sprintf(buf, "%08ld - ", millis());
  n  += vsprintf(buf + n, fmt, args);
  buf[n++] = '\n';
  va_end(args);

  i   = 0;
  do {
    l_buf[pin++] = buf[i++];
  } while ((i < n) && (pin != l_pout));

  if (pin != l_pout)
    l_pin = pin;
}

void
debug_run ( void )
{
  while ((l_pin != l_pout) && Serial.availableForWrite())
    Serial.write(l_buf[l_pout++]);
}

/* ****************************************************************************
 * Editor Configuration
 *
 * vim:sts=2:ts=2:sw=2:et
 * ***************************************************************************/
