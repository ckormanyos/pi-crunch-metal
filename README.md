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
![](./images/pi_crunch_rpi_zero.jpg)

TBD: Circuit description and pin connections.
