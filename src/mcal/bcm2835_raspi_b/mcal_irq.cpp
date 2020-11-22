///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2014 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_irq.h>

void mcal::irq::init(const config_type*)
{
  // Maskable interrupts are not used in this derivative.
  // Do not enable all global (maskable) interrupts.
}
