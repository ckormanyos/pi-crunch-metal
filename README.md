# pi-crunch-metal
Computes a million digits of pi on selected bare-metal microcontroller systems.

This fascinating, educational and fun project combines the domain
of high-performance numerical computing with the raw simplicity of
bare-metal embedded microcontroller systems.

Exihbiting the utmost in portability realized through modern header-only C++11
template code, pi-crunch-metal is intended to run out of the box on both
a PC system as well as selected bare-metal microcontroller systems.

The [wide-decimal](https://github.com/ckormanyos/wide-decimal) multiprecision
float back end provides the big-number engine for pi-crunch-metal.
Computation progress can be displayed on either the console (for PC systems)
or on a simple industry-standard LCD character display.

# Prototype Project

This repo features a fully-worked-out pi-crunch-metal prototype example project
running on a
[RaspberryPi(R)-Zero](https://www.raspberrypi.org/products/raspberry-pi-zero).
This powerful single-board computer is driven in OS-less, bare-metal mode
directly out-of-the-box.

The hardware setup is pictured in the image below.
In this image, the system has successfully completed one full
mega-digit pi calculation and is almost done with a second one.

![](./images/pi_crunch_rpi_zero.jpg)

Traditional wire-wrapping techniques connect the pins on a self-made
breakout board to a solderless prototyping breadboard.
Bit banging is used to implement an all-software SPI-compatible
driver which controls a port expander chip. The port
expander chip is used to control the pins on an industry-standard
LCD character display.

A classic logic-AND gate converts 3.3V to TTL 5V level.
A little sawed out aluminum chunk is used as a heat-sink to cool
the processor.

The output pin connections from the Rpi-Zero to the logic-AND gate
are shown in the table below.

| Rpi-Zero PIN  | AND-gate PINS   | SPI Function  |
| ------------- | --------------- | ------------- |
| GPIO16/H36    | in 10/9, out 8  | CS            |
| GPIO18/H12    | in 1/2, out 3   | SCK           |
| GPIO19/H35    | in 4/5, out 6   | MOSI          |

The LCD pin connections and the
input/output connections of the port expander chip are clearly
identifiable in the image.
