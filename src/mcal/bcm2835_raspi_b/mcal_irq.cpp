///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2014 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_cpu.h>

extern "C"
{
  void __int_vect_undef_instr_handler   (void);
  void __int_vect_prefetch_abort_handler(void);
  void __int_vect_data_abort_handler    (void);
  void __int_vect_unused_05_handler     (void);
  void OsCat1Isr                        (void);

  void __int_vect_undef_instr_handler   () { for(;;) { mcal::cpu::nop(); } }
  void __int_vect_prefetch_abort_handler() { for(;;) { mcal::cpu::nop(); } }
  void __int_vect_data_abort_handler    () { for(;;) { mcal::cpu::nop(); } }
  void __int_vect_unused_05_handler     () { for(;;) { mcal::cpu::nop(); } }
  void OsCat1Isr                        () { for(;;) { mcal::cpu::nop(); } }
}
