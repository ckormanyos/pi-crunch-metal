///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2021.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal/mcal.h>

// cd /mnt/c/Users/User/Documents/Ks/uC_Software/Boards/pi-crunch-metal
// ./target/build/build.sh bcm2835_raspi_b rebuild

namespace app { namespace benchmark {

bool run_pi_agm();

} } // namespace app::benchmark

namespace app { namespace led {
  void task_init();
  void task_func();

} } // namespace app::benchmark

#if defined(__GNUC__)
int main(void) __attribute__((used));
#endif

int main(void)
{
  // Initialize the microcontroller abstraction layer.
  mcal::init();

  app::led::task_init();

  bool result_is_ok = true;

  while(result_is_ok)
  {
    const bool result_pi_agm_is_ok = app::benchmark::run_pi_agm();

    result_is_ok &= result_pi_agm_is_ok;

    app::led::task_func();
  }

  if(result_is_ok == false)
  {
    for(;;) { ; }
  }
}
