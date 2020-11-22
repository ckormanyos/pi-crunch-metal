///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_cpu.h>
#include <mcal_cpu_detail_secure.h>

void mcal::cpu::init()
{
  detail::init();
}

void mcal::cpu::post_init()
{
}

void mcal::cpu::detail::init()
{
  mcal::cpu::detail::secure::initialize_vfp_unit();
  mcal::cpu::detail::secure::clear_branch_prediction_array();
  mcal::cpu::detail::secure::fill_tlb_entries();
  mcal::cpu::detail::secure::enable_mmu();
  mcal::cpu::detail::secure::enable_branch_prediction();
  mcal::cpu::detail::secure::enable_caches();
}
