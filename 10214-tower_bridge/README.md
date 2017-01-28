10214 Tower Bridge
==================

This is my first endeavour at lighting and animating my LEGO models. The plan
was to motorise both halves of the bridge and to add lots of LEDs to the main structure.

This was also my first electronics project, so tried to keep things fairly simple. Originally the intention was just to build something out of veriboard or similar, but a friend showed my how easy it was to design and schematic / PCB and get it fabricated for minimal cost.

Mechanics
---------

The design has 2 stepper motors, one for each half of the bridge. These are hidden away under the road section of each tower. Some minor re-arrangement of the internal structure was needed to make space and also additional brick work was added to hold the motors in place.

I also removed the original pivots as they weren't designed to be moved automatically and presented too much friction. A simple technic bar and end plugs was used to replce this. Some standard bricks were also replaced with technic (holed) bricks to allow wires to run.

Each stepper had a technic circular disc pushed onto the spindle to help guide the winding of the cable.

Electronics
-----------

A simple PCB comprising an arduino, darlington array (to provide 5V signals to the motor), connectors for sensors, tower interconnect, motor and LEDs and 220ohm resistor for each LED (I didn't do a proper job and match, it's good enough).

There is also a string of LEDs in the top of the bridge crossing between the two towers. These have inline resistors as it I didn't want to have to run lots of cables from the PCB!

The two boards are then interconnected by a 3-wire cable carrying GND, VCC (5v) and RX (RS232). The serial RX acts as a receive only "bus", allowing me to address both towers simultaneously. There is no feedback, but none was required.

The interconnect header on one tower also doubles up as the means of external connection to the power supply and control board (an rpi).

Firmware
--------

The easy bit :) although as usual being software I ended up over-engineering and eventually scrapped lots of it to keep things simple.

The arduino monitors the end stop sensors and controls the bridge autonomously. It has the option to move to top, bottom or a specific position (based on stepper steps).

There is also individual control of most of the LEDs (total overkill, but I had lots of spare IO, so what the heck). The only ones that aren't individually controllable are the ones spanning the two bridges.
