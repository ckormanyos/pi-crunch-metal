///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_PORT_2012_06_27_H
  #define MCAL_PORT_2012_06_27_H

  #if !defined(__cplusplus)
  #include <stdint.h>
  #endif

  #if defined(__cplusplus)

  #include <mcal_port_expander_base.h>
  #include <mcal_reg.h>

  namespace mcal
  {
    namespace port
    {
      auto port_expander_device() -> port_expander_base&;

      inline auto port_expander_set_direction_output(const uint8_t bpos) -> void { port_expander_device().set_direction_output(bpos); }
      inline auto port_expander_set_direction_input (const uint8_t bpos) -> void { port_expander_device().set_direction_input(bpos); }
      inline auto port_expander_set_pin_high        (const uint8_t bpos) -> void { port_expander_device().set_pin_high(bpos); }
      inline auto port_expander_set_pin_low         (const uint8_t bpos) -> void { port_expander_device().set_pin_low(bpos); }
      inline auto port_expander_read_input_value    (const uint8_t bpos) -> bool { static_cast<void>(bpos); return false; }
      inline auto port_expander_toggle_pin          (const uint8_t bpos) -> void { port_expander_device().toggle_pin(bpos); }

      constexpr auto gpio_pin02_h03 = static_cast<std::uint8_t>(UINT8_C(0x02));
      constexpr auto gpio_pin03_h05 = static_cast<std::uint8_t>(UINT8_C(0x03));
      constexpr auto gpio_pin04_h07 = static_cast<std::uint8_t>(UINT8_C(0x04));
      constexpr auto gpio_pin17_h11 = static_cast<std::uint8_t>(UINT8_C(0x17));
      constexpr auto gpio_pin27_h13 = static_cast<std::uint8_t>(UINT8_C(0x27));
      constexpr auto gpio_pin22_h15 = static_cast<std::uint8_t>(UINT8_C(0x22));
      constexpr auto gpio_pin10_h19 = static_cast<std::uint8_t>(UINT8_C(0x10));
      constexpr auto gpio_pin09_h21 = static_cast<std::uint8_t>(UINT8_C(0x09));
      constexpr auto gpio_pin11_h23 = static_cast<std::uint8_t>(UINT8_C(0x11));
      constexpr auto gpio_pin05_h29 = static_cast<std::uint8_t>(UINT8_C(0x05));
      constexpr auto gpio_pin06_h31 = static_cast<std::uint8_t>(UINT8_C(0x06));
      constexpr auto gpio_pin13_h33 = static_cast<std::uint8_t>(UINT8_C(0x13));
      constexpr auto gpio_pin19_h35 = static_cast<std::uint8_t>(UINT8_C(0x19));
      constexpr auto gpio_pin26_h37 = static_cast<std::uint8_t>(UINT8_C(0x26));
      constexpr auto gpio_pin14_h08 = static_cast<std::uint8_t>(UINT8_C(0x14));
      constexpr auto gpio_pin15_h10 = static_cast<std::uint8_t>(UINT8_C(0x15));
      constexpr auto gpio_pin18_h12 = static_cast<std::uint8_t>(UINT8_C(0x18));
      constexpr auto gpio_pin23_h16 = static_cast<std::uint8_t>(UINT8_C(0x23));
      constexpr auto gpio_pin24_h18 = static_cast<std::uint8_t>(UINT8_C(0x24));
      constexpr auto gpio_pin25_h22 = static_cast<std::uint8_t>(UINT8_C(0x25));
      constexpr auto gpio_pin08_h24 = static_cast<std::uint8_t>(UINT8_C(0x08));
      constexpr auto gpio_pin07_h26 = static_cast<std::uint8_t>(UINT8_C(0x07));
      constexpr auto gpio_pin12_h32 = static_cast<std::uint8_t>(UINT8_C(0x12));
      constexpr auto gpio_pin16_h36 = static_cast<std::uint8_t>(UINT8_C(0x16));
      constexpr auto gpio_pin20_h38 = static_cast<std::uint8_t>(UINT8_C(0x20));
      constexpr auto gpio_pin21_h40 = static_cast<std::uint8_t>(UINT8_C(0x21));

      constexpr auto gpio_status_led = static_cast<std::uint8_t>(UINT8_C(0x47));

      class port_pin_base
      {
      protected:
        static constexpr auto gpfsel0_addr         = static_cast<std::uint32_t>(UINT32_C(0x20200000));

        static constexpr auto gpio_pin_x_input     = static_cast<std::uint32_t>(UINT8_C(0));
        static constexpr auto gpio_pin_x_output    = static_cast<std::uint32_t>(UINT8_C(1));

        static constexpr auto gpclr0_addr          = static_cast<std::uint32_t>(UINT32_C(0x20200028));
        static constexpr auto gpclr1_addr          = static_cast<std::uint32_t>(UINT32_C(0x2020002C));

        static constexpr auto gpset0_addr          = static_cast<std::uint32_t>(UINT32_C(0x2020001C));
        static constexpr auto gpset1_addr          = static_cast<std::uint32_t>(UINT32_C(0x20200020));

        static constexpr auto gpio_no_effect       = static_cast<std::uint32_t>(UINT8_C(0));
        static constexpr auto gpio_set_gpio_pin_x  = static_cast<std::uint32_t>(UINT8_C(1));
      };

      template<const std::uint8_t GpioPin>
      class port_pin : public port_pin_base
      {
      private:
        static bool my_pin_is_high;

        static constexpr auto gpio_pin = static_cast<std::uint8_t>(GpioPin);

        static constexpr auto port =
          static_cast<std::uint32_t>
          (
               static_cast<std::uint32_t>(gpio_pin & static_cast<std::uint32_t>(UINT8_C(0xF0)))
            >> static_cast<unsigned>(UINT8_C(4))
          );

        static constexpr auto pin =
          static_cast<std::uint32_t>
          (
            gpio_pin & static_cast<std::uint32_t>(UINT8_C(0x0F))
          );

        static constexpr auto pos =
          static_cast<std::uint32_t>
          (
              static_cast<std::uint32_t>(port * static_cast<std::uint32_t>(UINT8_C(10)))
            + pin
          );

        static constexpr auto addfsel =
          static_cast<std::uint32_t>
          (
              gpfsel0_addr
            + static_cast<std::uint32_t>(static_cast<std::uint32_t>(UINT8_C(4)) * port)
          );

        static constexpr auto valfout =
          static_cast<std::uint32_t>
          (
            gpio_pin_x_output << static_cast<unsigned>(pin * static_cast<std::uint32_t>(UINT8_C(3)))
          );

        static constexpr auto addclr =
          static_cast<std::uint32_t>
          (
            (gpio_pin < static_cast<std::uint8_t>(UINT8_C(32))) ? gpclr0_addr : gpclr1_addr
          );

        static constexpr auto pos_is_lt_32 = (pos < static_cast<std::uint32_t>(UINT8_C(32)));

        static constexpr auto shl_amount =
          static_cast<unsigned>
          (
            pos_is_lt_32 ? static_cast<unsigned>(pos)
                         : static_cast<unsigned>(pos - static_cast<std::uint32_t>(UINT8_C(32)))
          );

        static constexpr auto addset =
          static_cast<std::uint32_t>
          (
            pos_is_lt_32 ? gpset0_addr : gpset1_addr
          );

        static constexpr auto valclr = static_cast<std::uint32_t>(gpio_set_gpio_pin_x << shl_amount);
        static constexpr auto valset = valclr;

      public:
        static auto set_direction_output() -> void { mcal_reg_access32_reg_or(addfsel, valfout); }
        static auto set_direction_input () -> void { } // TBD: Not yet implemented.
        static auto set_pin_high        () -> void { mcal_reg_access32_reg_or(addset, valset); my_pin_is_high = true; }
        static auto set_pin_low         () -> void { mcal_reg_access32_reg_or(addclr, valclr); my_pin_is_high = false; }
        static auto read_input_value    () -> bool { return false; } // TBD: Not yet implemented.
        static auto toggle_pin          () -> void { (my_pin_is_high ? set_pin_low() : set_pin_high()); }
      };

      template<const std::uint8_t PortPin>
      bool port_pin<PortPin>::my_pin_is_high;

      template<const std::uint8_t bpos>
      class port_pin_expander
      {
      public:
        static auto set_direction_output() -> void {        port_expander_set_direction_output(bpos); }
        static auto set_direction_input () -> void {        port_expander_set_direction_input (bpos); }
        static auto set_pin_high        () -> void {        port_expander_set_pin_high        (bpos); }
        static auto set_pin_low         () -> void {        port_expander_set_pin_low         (bpos); }
        static auto read_input_value    () -> bool { return port_expander_read_input_value    (bpos); }
        static auto toggle_pin          () -> void {        port_expander_toggle_pin          (bpos); }
      };
    }
  }
  #endif

  #if defined(__cplusplus)
  extern "C"
  {
  #endif

  void mcal_port_pin_clr(uint8_t PortPin);
  void mcal_port_pin_set(uint8_t PortPin);

  #if defined(__cplusplus)
  }
  #endif

#endif // MCAL_PORT_2012_06_27_H
