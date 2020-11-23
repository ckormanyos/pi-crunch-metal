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

TBD: Pin connections.
