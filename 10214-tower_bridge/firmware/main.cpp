#include "Arduino.h"
#include "HardwareSerial.h"

#include "stepper.h"
#include "bridge.h"
#include "led.h"
#include "log.h"

int  IDX = 0;
char BUF[128];

void
setup ( void )
{
  Serial.begin(9600);
  debug("starting...");
  stepper_init();
  bridge_init();
  led_init();
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void
proc ( const char *line )
{
  if (0 == strcmp(line, "is_up")) {
  } else if (0 == strcmp(line, "is_down")) {
  } else if (0 == strcmp(line, "ping")) {
    debug("pong");
  } else if (0 == strcmp(line, "up")) {
    bridge_up(0);
  } else if (0 == strcmp(line, "down")) {
    bridge_down(0);
  } else if (0 == strcmp(line, "stop")) {
    bridge_stop();
  } else if (0 == strncmp(line, "pos", 3)) {
    int pos;
    sscanf(line+4, "%d", &pos);
    bridge_position(pos, 0);
  } else if (0 == strncmp(line, "lmask", 5)) {
    int leds;
    sscanf(line+6, "%x", &leds);
    led_mask((uint16_t)leds);
  } else if (0 == strncmp(line, "lon", 3)) {
    int led;
    sscanf(line+4, "%d", &led);
    led_on((uint8_t)led);
  } else if (0 == strncmp(line, "loff", 4)) {
    int led;
    sscanf(line+5, "%d", &led);
    led_off((uint8_t)led);
  } else if (0 == strncmp(line, "ltog", 4)) {
    int led;
    sscanf(line+5, "%d", &led);
    led_toggle((uint8_t)led);
  }
}

void
loop ( void )
{
  debug_run();
  stepper_run();
  bridge_run();
  
  int x = Serial.read();
  if ('\r' == x) {
  } else if ('\n' == x) {
    BUF[IDX] = '\0';
    proc(BUF);
    IDX = 0;
  } else if (-1 != x) {
    BUF[IDX] = x;
    IDX = (IDX + 1) % 128;
  }

  if ((millis() % 1000) < 500) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }

#if 0
  if ((millis() % 1000) == 0) {
    debug("loop");
  }
#endif
}
