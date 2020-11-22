///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_cpu_detail_secure.h>

void mcal_cpu_detail_secure_start_mmu(uint32_t tlb_base, uint32_t mmu_flags)
{
  asm volatile("mov r2, #0");
  asm volatile("mcr p15, 0, r2, c7, c7, 0");  // @; invalidate caches
  asm volatile("mcr p15, 0, r2, c8, c7, 0");  // @; invalidate tlb
  asm volatile("mcr p15, 0, r2, c7, c10, 5"); // @; DSB (barrier instruction)

  asm volatile("mvn r2, #0");
  asm volatile("bic r2, #0xC");
  asm volatile("mcr p15, 0, r2, c3, c0, 0");  // @; domain

  asm volatile("mcr p15, 0, r0, c2, c0, 0");  // @; tlb base
  asm volatile("mcr p15, 0, r0, c2, c0, 1");  // @; tlb base

  asm volatile("mrc p15, 0, r2, c1, c0, 0");
  asm volatile("orr r2, r2, r1");
  asm volatile("mcr p15, 0, r2, c1, c0, 0");

  asm volatile("bx lr");
}
