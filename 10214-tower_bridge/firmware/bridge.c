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
 * Bridge control
 *
 * ***************************************************************************/

#include "stepper.h"
#include "bridge.h"
#include "log.h"

#include <Arduino.h>
#include <avr/eeprom.h>
#include <stdint.h>
#include <stdbool.h>

#define BRIDGE_DW_PIN        (11)
#define BRIDGE_UP_PIN        (12)
#define BRIDGE_SAFETY_TIME   (20000)
#define BRIDGE_DEFAULT_SPEED (8)
#define BRIDGE_SLOW_SPEED    (2)

/*
 * State
 */
enum {
  BR_IDLE,
  BR_UP,
  BR_DW,
  BR_POS,
  BR_FAIL,

  BR_CALIB_INIT_UP,
  BR_CALIB_INIT_DW,
  BR_CALIB_INIT_UN,
  BR_CALIB_FINISH,

} br_state;

struct {
  uint16_t bc_max_move;
  int8_t   bc_up_dir;
  int8_t   bc_dw_dir;
  uint8_t  bc_valid;
} br_calib = {
  .bc_max_move = 0,
  .bc_up_dir   = 0,
  .bc_dw_dir   = 0,
  .bc_valid    = 0,
};

int32_t  br_fail = -1; // TODO: not handling wrap
uint32_t br_start = 0;

/**
 * Check if bridge is up
 */
static bool
bridge_is_up ( void )
{
  return (0 == digitalRead(BRIDGE_UP_PIN));
}

/**
 * Check if bridge is down
 */
static bool
bridge_is_down ( void )
{
  return (0 == digitalRead(BRIDGE_DW_PIN));
}

/**
 * Request the bridge goes UP
 */
void
bridge_up ( uint32_t rpm )
{
  if (BR_FAIL <= br_state) return;

  if (!bridge_is_up()) {
    debug("bridge - going UP");
    br_state = BR_UP;
    br_fail  = millis() + BRIDGE_SAFETY_TIME;//br_calib.bc_max_move;
    stepper_move(br_calib.bc_up_dir, rpm ? rpm : BRIDGE_DEFAULT_SPEED, 0);
  }
}

/**
 * Request the bridge goes DOWN
 */
void
bridge_down ( uint32_t rpm )
{
  if (BR_FAIL <= br_state) return;

  if (!bridge_is_down()) {
    debug("bridge - going DOWN");
    br_state = BR_DW;
    br_fail  = millis() + BRIDGE_SAFETY_TIME;//br_calib.bc_max_move;
    stepper_move(br_calib.bc_dw_dir, rpm ? rpm : BRIDGE_DEFAULT_SPEED, 0);
  }
}

/**
 * Stop
 */
void
bridge_stop ( void )
{
  if (BR_IDLE != br_state) {
    debug("bridge - STOPping");
    br_state = BR_IDLE;
    br_fail  = -1;
  }
  stepper_stop();
}

/**
 * Move to position
 */
void
bridge_position ( uint32_t pos, uint32_t rpm )
{
  uint32_t steps;
  uint32_t cpos = stepper_get_pos();
  debug("pos %d step_pos %d", (int)pos, (int)cpos);
  if (pos > cpos) {
    steps = pos - cpos;
    debug("bridge - going UP %d", steps);
    br_state = BR_POS;
    br_fail  = millis() + BRIDGE_SAFETY_TIME;
    stepper_move(br_calib.bc_up_dir, rpm ? rpm : BRIDGE_DEFAULT_SPEED, steps);
  } else if (pos < cpos) {
    steps = cpos - pos;
    debug("bridge - going DOWN %d", steps);
    br_state = BR_POS;
    br_fail  = millis() + BRIDGE_SAFETY_TIME;
    stepper_move(br_calib.bc_dw_dir, rpm ? rpm : BRIDGE_DEFAULT_SPEED, steps);
  }
}

/**
 * Step
 */
void
bridge_run ( void )
{
  uint32_t now = millis();

  /* Override */
  if (BR_IDLE == br_state) {
    br_fail = -1;
    //stepper_stop();
    return;
  }

  /* Failed STOP!! */
  if (-1 != br_fail && (now >= br_fail)) {

    /* Reverse direction, and try again */
    if ((BR_CALIB_INIT_UP == br_state) && (1 == stepper_get_dir())) {
      stepper_move(-1, BRIDGE_SLOW_SPEED, 0);
      br_fail = now + 2000;
      debug("bridge: calib UP, wrong direction?");
      return;
    }

    debug("bridge: sensors failed, STOP!!!");
    stepper_stop();
    br_state = BR_FAIL;
    br_fail  = -1;
    return;
  }

  switch ( br_state ) {
    case BR_CALIB_INIT_UP:
      if (bridge_is_up()) return;
      debug("bridge: calib, going DOWN");

      /* Reverse direction and start from TOP */
      stepper_move(-stepper_get_dir(), BRIDGE_DEFAULT_SPEED, 0);
      br_state = BR_CALIB_INIT_UN;
      br_fail  = now + 500;
      break;

    case BR_CALIB_INIT_DW:
      if (bridge_is_down()) return;
      debug("bridge: calib, going UP");

      /* Note: we need bridge to go to TOP */
      br_state = BR_CALIB_INIT_UN;
      br_fail  = now + BRIDGE_SAFETY_TIME;
      break;

    case BR_CALIB_INIT_UN:
      if (bridge_is_down()) {
        debug("bridge: calib, found BOTTOM");

        /* Note: we need bridge to go to TOP 
         *       we pretend we're starting from DOWN, and we'll come back
         *       here once we're on our way up
         */
        stepper_move(-stepper_get_dir(), BRIDGE_DEFAULT_SPEED, 0);
        br_state = BR_CALIB_INIT_DW;
        br_fail  = now + BRIDGE_SAFETY_TIME;

      } else if (bridge_is_up()) {
        debug("bridge: calib, found TOP");
        br_calib.bc_up_dir = stepper_get_dir();
        br_calib.bc_dw_dir = -stepper_get_dir();

        /* Finished, now reverse direction and measure time */
        br_state           = BR_CALIB_FINISH;
        br_fail            = now + BRIDGE_SAFETY_TIME;
        br_start           = now;
        stepper_move(-stepper_get_dir(), BRIDGE_DEFAULT_SPEED, 0);

      } else {
        // waiting
      }
      break;

    case BR_CALIB_FINISH:

      /* Done */
      if (bridge_is_down()) {
        br_calib.bc_max_move = ((now - br_start) * 110) / 100;
        br_calib.bc_valid    = 0x69;
        eeprom_write_block(&br_calib, 0x0, sizeof(br_calib));
        br_state = BR_IDLE;
        stepper_stop();
        stepper_zero_pos();
        debug("bridge: calib finished, max time %d", (int)br_calib.bc_max_move);
      }
      break;

    case BR_UP:
      if (bridge_is_up()) {
        debug("bridge: is UP");
        br_state = BR_IDLE;
        stepper_stop();
      }
      break;
    
    case BR_DW:
      if (bridge_is_down()) {
        debug("bridge: is DOWN");
        br_state = BR_IDLE;
        stepper_stop();
        stepper_zero_pos();
      }
      break;

    case BR_POS:
      if (0 == stepper_get_dir()) {
        debug("bridge: at POS");
        br_state = BR_IDLE;
        stepper_stop();
      }
      break;

    default:
      br_fail  = -1;
      br_state = BR_IDLE;
      stepper_stop();
      break;
  }
}

/**
 * Initialise
 */
void
bridge_init ( void )
{
  uint32_t now = millis();

  /* Initialise the sensor pins */
  pinMode(BRIDGE_UP_PIN, INPUT_PULLUP);
  pinMode(BRIDGE_DW_PIN, INPUT_PULLUP);

  /* Load calib */
  eeprom_read_block(&br_calib, 0x0, sizeof(br_calib));

  /* Need to calib */
  if (0x69 != br_calib.bc_valid) {

    /* Bridge UP!!
     *
     * This is dangerous, over straining the cable could break something
     * however we don't know which way is DOWN! So we turn it for a short
     * period and if nothing happens we go the other way
     */
    if (bridge_is_up()) {
      debug("bridge: calib from UP");
      br_state = BR_CALIB_INIT_UP;
      br_fail  = now + 200;
      stepper_move(1, BRIDGE_SLOW_SPEED, 0);
    
    /* Bridge DOWN
     *
     * This is relatively safe, we can just turn until it rises, whichever
     * way we turn it should eventually rise
     */
    } else if (bridge_is_down()) {
      debug("bridge: calib from DW");
      br_state = BR_CALIB_INIT_DW;
      br_fail  = now + BRIDGE_SAFETY_TIME;
      stepper_move(1, BRIDGE_DEFAULT_SPEED, 0);
    
    /* Bridge is neither
     *
     * Even simpler, just turn it till we hit an end stop
     */
    } else {
      br_state = BR_CALIB_INIT_UN;
      br_fail  = now + BRIDGE_SAFETY_TIME;
      stepper_move(1, BRIDGE_DEFAULT_SPEED, 0);
      debug("bridge: calib from UN");
    }

  /* Already calib */
  } else {
    bridge_down(0); // Always return to down
    br_fail  = now + br_calib.bc_max_move;
    debug("bridge: ready");
  }
}
