///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_port.h>
#include <mcal_port_expander_microchip_mcp23s17.h>

auto mcal::port::port_expander_device() -> mcal::port::port_expander_base&
{
  using local_port_pin_expander_type =
    mcal::port::port_pin_expander_microchip_mcp23s17<static_cast<std::uint8_t>(UINT8_C(2))>;

  static local_port_pin_expander_type
    port_pin_expander_device
    {
      mcal::spi::spi1()
    };

  return port_pin_expander_device;
}
