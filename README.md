pi-crunch-metal
==================

<p align="center">
    <a href="https://github.com/ckormanyos/pi-crunch-metal/actions">
        <img src="https://github.com/ckormanyos/pi-crunch-metal/actions/workflows/pi-crunch-metal.yml/badge.svg" alt="Build Status"></a>
    <a href="https://github.com/ckormanyos/pi-crunch-metal/issues?q=is%3Aissue+is%3Aopen+sort%3Aupdated-desc">
        <img src="https://custom-icon-badges.herokuapp.com/github/issues-raw/ckormanyos/pi-crunch-metal?logo=github" alt="Issues" /></a>
    <a href="https://github.com/ckormanyos/pi-crunch-metal/blob/master/LICENSE_1_0.txt">
        <img src="https://img.shields.io/badge/license-BSL%201.0-blue.svg" alt="Boost Software License 1.0"></a>
    <a href="https://github.com/ckormanyos/pi-crunch-metal">
        <img src="https://img.shields.io/github/languages/code-size/ckormanyos/pi-crunch-metal" alt="GitHub code size in bytes" /></a>
</p>

pi-crunch-metal computes a million digits of $\pi$
on a bare-metal RaspberryPi(R)-Zero (and other systems).

This fascinating, educational and fun project combines the domain
of high-performance numerical computing with the raw simplicity of
bare-metal embedded microcontroller systems.

Exihbiting the utmost in portability realized primarily through modern header-only C++14
template code, pi-crunch-metal is intended to run out of the box on both
a PC system as well as selected bare-metal microcontroller systems.

The [wide-decimal](https://github.com/ckormanyos/wide-decimal) multiprecision
float back end provides the big-number engine for pi-crunch-metal.
Computation progress can be displayed on either the console (for PC systems)
or on a simple industry-standard LCD character display.

The backbone real-time operating system is taken directly
from the OSEK-like OS implemented in
[Chalandi/OSEK_Raspberry_Pi_Zero](https://github.com/Chalandi/OSEK_Raspberry_Pi_Zero)

# Software Details

## AGM Algorithm

In this project, $1,000,001$ decmal digits of $\pi$ (i.e., one-million-and-1)
are computed with a quadratically converging Gauss arithmetic geometric mean
iteration.

Multiplication is the hot-spot of this program. The multiplication
implementation uses a combination of school multiplication for low
precision and Karatsuba multiplication for medium percision,
switching directly over to an FFT multiplication scheme
for higher precision.

The microcontroller boots and performs static initialization via self-written
startup code. Hardware setup including clock initialization,
FPU enable, instruction caching, etc. is carried out with self-written
hybrid assembly/C++ code shortly after reaching `main()`.

Compact code size is in focus and the entire project easily fits within 40k
of program code, with slight variations depending on the optimization settings,
compiler and target system selected.
The calculation does, however, require ample RAM of about 16 Mbyte.

GNU/GCC `gcc-arm-non-eabi` is used for the target system.
Cross development with `Win*` and/or `*nix` host is
supported. Build tools, the build system and the compilers
are essentially the same as those used in the
[`real-time-cpp`](https://github.com/ckormanyos/real-time-cpp)
repository.

## Alternate Spigot Algorithm

In addition to the AGM algorithm mentioned above, a slower
quadratic pi-spigot algorithm of order $N^2$ is also supported
in this project.
Switching to use the (interchangeable) file
[pi_spigot.cpp](./src/app/Appli/pi_spigot/pi_spigot.cpp)
instead of the default
[pi_agm.cpp](./src/app/Appli/pi_agm/pi_agm.cpp)
alternately uses the pi-spigot algorithm.

The spigot calculation
is slower than the AGM calculation and requires sightly
more time to compute a count of $100,001$ decimal digits,
a factor of ten fewer.

# Prototype Project

This repo features a fully-worked-out pi-crunch-metal prototype example project
running on a
[RaspberryPi(R)-Zero](https://www.raspberrypi.org/products/raspberry-pi-zero).
This powerful single-board computer is driven in OS-less, bare-metal mode
directly out-of-the-box.

The build system is set up to use GCC, making use of the `arm-none-eabi`
compiler taken directly from the
[real-time-cpp-toolchains](https://github.com/ckormanyos/real-time-cpp-toolchains)
repository.

The default optimization setting is `-O2` and the million-and-one
decimal digit pi calculation takes slightly under 30 minutes
on this target system with the above-mentioned compiler.

The hardware setup is pictured in the image below.
In this image, the system has successfully completed one full
mega-digit pi calculation and is almost done with a second one.

![](./images/pi_crunch_rpi_zero.jpg)

Traditional wire-wrapping techniques connect the pins on a self-made
breakout board to a solderless prototyping breadboard.
Double and quadruple strands of skinny wire are used on the
power and ground pins, as these might typically carry up to 150mA
of current in this setup.

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
identifiable in the image. The port expander chip uses
hardware addressing and is hard-wired to address 2.
