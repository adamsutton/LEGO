# ####################################################################
# Blinky exmaple
# ####################################################################

cmake_minimum_required(VERSION 3.5)

project(tower_bridge)

#
# Configure board
#

set(ARDUINO_DEFAULT_BOARD nano)
set(ARDUINO_DEFAULT_PORT  /dev/ttyUSB0)

#
# Firmware

generate_arduino_firmware(
  tower_bridge

  BOARD atmega328
  PORT  /dev/ttyUSB0

  SRCS  main.cpp log.cpp stepper.c bridge.c led.c
)
