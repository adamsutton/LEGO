/* *************************************************************************** *
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
 * Stepper motor driver for BYJ48
 *
 * ***************************************************************************/

#include "stepper.h"
#include "log.h"

#include <Arduino.h>

/* Number of steps per REV */
#define STEP_PER_REV (4096)

/* Convert RPM to microseconds per step */
static inline uint32_t
step_rpm_to_us ( uint32_t rpm )
{
  return 60000000L / (STEP_PER_REV * rpm);
}

/* Motor control pins */
static const int MOTOR_PINS[]   = { 7, 8, 9, 10 };

/* Motor control pin sequence(s) */
static const int MOTOR_SEQ[][4] = {
  {1, 0, 0, 1},
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
};

/* State */
uint32_t step_rpm   = 0;
uint32_t step_tick  = 0;
int8_t   step_dir   = 0;
uint8_t  step_idx   = 0;
uint32_t step_last  = 0;
uint32_t step_pos   = 0;
uint32_t step_limit = 0;

/* Stop */
void
stepper_stop ( void )
{
  step_dir = 0;
  step_rpm = 0;
  //if ((0 != step_dir) || (0 != step_rpm)) {
    debug("stepper: stop @ %d", step_pos);
  //}
}

/* Clockwise */
void
stepper_move ( int8_t dir, uint32_t rpm, uint32_t steps )
{
  if (0 == dir) {
    stepper_stop();
  } else {
    step_dir   = dir;
    step_rpm   = rpm;
    step_tick  = step_rpm_to_us(step_rpm);
    step_last  = micros();
    step_limit = steps;
    debug("stepper: move dir %d rpm %ld tick %ld last %ld",
          step_dir, step_rpm, step_tick, step_last);
  }
}

/*
 * Current direction
 */
int8_t
stepper_get_dir ( void)
{
  return step_dir;
}

/*
 * Current RPM
 */
uint32_t
stepper_get_rpm ( void )
{
  return step_rpm;
}

/*
 * Current position
 */
uint32_t
stepper_get_pos ( void )
{
  debug("stepper: pos %d", step_pos);
  return step_pos;
}

/*
 * Zero the position
 */
void
stepper_zero_pos ( void )
{
  step_pos = 0;
}

/*
 * Run
 */
void
stepper_run ( void )
{
  int i;
  uint32_t us, now;
  
  /* Ignore */
  if ((0 == step_rpm) || (0 == step_dir))
    return;

  /* Get delta */
  now = micros();
  us  = now - step_last;

  while (us >= step_tick) {

    us       -= step_tick;
    step_tick = step_rpm_to_us(step_rpm);

    /* Update */
    step_idx += (8 + step_dir);
    step_idx %= 8;

    /* Output */
    for (i = 0; i < 4; i++) {
      digitalWrite(MOTOR_PINS[i], MOTOR_SEQ[step_idx][i]);
    }

    /* Update pos (don't allow negative) */
    step_pos += step_dir;
    if (0xFFFFFFFF == step_pos) step_pos = 0;

    /* Limit reached, stop */
    if (step_limit > 0) {
      --step_limit;
      if (0 == step_limit) {
        stepper_stop();
      }
    }
  }

  /* Remove what's left */
  step_tick -= us;
  step_last  = now;
}

/*
 * Set everything up
 */
void
stepper_init ( void )
{
  int i;

  /* Setup the motor pins */
  for (i = 0; i < 4; i++) {
    pinMode(MOTOR_PINS[i], OUTPUT);
    digitalWrite(MOTOR_PINS[i], LOW);
  }
}
