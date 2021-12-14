///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2021.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal/mcal.h>
#include <util/utility/util_time.h>

namespace app
{
  namespace led
  {
    void task_init();
    void task_func();
  }
}

void app::led::task_init()
{
  mcal::led::led0().toggle();
}

void app::led::task_func()
{
  mcal::led::led0().toggle();
}
