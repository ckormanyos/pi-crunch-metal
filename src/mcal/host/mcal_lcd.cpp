///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_lcd.h>
#include <mcal_lcd/mcal_lcd_console.h>

void mcal::lcd::init(const config_type*)
{
  static_cast<void>(lcd0().init());
}

mcal::lcd::lcd_base& mcal::lcd::lcd0()
{
  static mcal::lcd::lcd_console lc0;

  return lc0;
}
