///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Raspberry Pi bare-metal startup code.
// Expressed with C++ for Raspberry Pi bare-metal by Chris.

#include <mcal_cpu.h>

namespace crt
{
  void init_ram  () __attribute__((section(".startup"), used, noinline));
  void init_ctors() __attribute__((section(".startup"), used, noinline));
}

extern "C" void __my_startup(void) __attribute__((section(".startup"), used, noinline));

extern "C" void __my_startup(void)
{
  // Initialize statics from ROM to RAM.
  // Zero-clear default-initialized static RAM.
  crt::init_ram();

  // Call all ctor initializations.
  crt::init_ctors();

  // Initialize the CPU (including FPU, caches, MMU, etc.).
  // This is only done *after* initializing RAM/ROM in porder
  // to keep initialization code and memory regions out
  // of cached data and instructions.

  mcal::cpu::init();

  // Jump to main (and never return).
  asm volatile("b main");

  // Catch an unexpected return from main.
  for(;;)
  {
    // Replace with a loud error if desired.
    ;
  }
}
