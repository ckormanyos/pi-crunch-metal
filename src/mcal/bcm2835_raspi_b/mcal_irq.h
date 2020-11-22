///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2014 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_IRQ_2014_09_20_H_
  #define MCAL_IRQ_2014_09_20_H_

  namespace mcal
  {
    namespace irq
    {
      typedef void config_type;
      void init(const config_type*);

      inline void enable_all()
      {
        // Enable all maskable interrupts.
        asm volatile("cpsie i");
      }

      inline void disable_all()
      {
        // Disable all maskable interrupts.
        asm volatile("cpsid i");
      }
    }
  }

#endif // MCAL_IRQ_2014_09_20_H_
