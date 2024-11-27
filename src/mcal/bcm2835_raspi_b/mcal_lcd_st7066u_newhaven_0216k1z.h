///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2024.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_LCD_ST7066U_NEWHAVEN_0216K1Z_2020_05_07_H_
  #define MCAL_LCD_ST7066U_NEWHAVEN_0216K1Z_2020_05_07_H_

  #include <algorithm>
  #include <cstdint>
  #include <cstring>

  #include <mcal_lcd/mcal_lcd_base.h>
  #include <util/utility/util_time.h>

  namespace mcal { namespace lcd {

  template<typename port_pin_rs__type,
           typename port_pin_rw__type,
           typename port_pin_e___type,
           typename port_pin_db0_type,
           typename port_pin_db1_type,
           typename port_pin_db2_type,
           typename port_pin_db3_type,
           typename port_pin_db4_type,
           typename port_pin_db5_type,
           typename port_pin_db6_type,
           typename port_pin_db7_type>
  class lcd_st7066u_newhaven_0216k1z final : public mcal::lcd::lcd_base
  {
  private:
    using timer_type = util::timer<std::uint32_t>;

  public:
    auto init() noexcept -> bool override
    {
      port_pin_rs__type::set_pin_low();
      port_pin_rw__type::set_pin_low();
      port_pin_e___type::set_pin_high();

      P1_set(UINT8_C(0));

      port_pin_rs__type::set_direction_output();
      port_pin_rw__type::set_direction_output();
      port_pin_e___type::set_direction_output();

      P1_set_direction_output();

      port_pin_e___type::set_pin_low();

      blocking_delay(timer_type::milliseconds(15U));                                   // Set a timer which is at least 15ms from system start.
      command(static_cast<std::uint8_t>(UINT8_C(0x30)));                               // Command 0x30 = Wake up
      blocking_delay(timer_type::milliseconds(static_cast<unsigned>(UINT8_C(7))));     // Must wait 5ms, busy flag not available.
      command(static_cast<std::uint8_t>(UINT8_C(0x30)));                               // Command 0x30 = Wake up 2
      blocking_delay(timer_type::microseconds(static_cast<unsigned>(UINT8_C(200))));   // Must wait 160us, busy flag not available
      command(static_cast<std::uint8_t>(UINT8_C(0x30)));                               // Command 0x30 = Wake up 3
      blocking_delay(timer_type::microseconds(static_cast<unsigned>(UINT8_C(200))));   // Must wait 160us, busy flag not available
      command(static_cast<std::uint8_t>(UINT8_C(0x38)));                               // Function set: 8-bit/2-line
      command(static_cast<std::uint8_t>(UINT8_C(0x10)));                               // Set cursor
      command(static_cast<std::uint8_t>(UINT8_C(0x0C)));                               // Display ON; Cursor ON
      command(static_cast<std::uint8_t>(UINT8_C(0x06)));                               // Entry mode set

      clear_line(static_cast<std::uint8_t>(UINT8_C(3)));
      clear_line(static_cast<std::uint8_t>(UINT8_C(2)));
      clear_line(static_cast<std::uint8_t>(UINT8_C(1)));
      clear_line(static_cast<std::uint8_t>(UINT8_C(0)));

      return true;
    }

    auto write(const char* pstr,
               const std::size_t length,
               const std::uint_fast8_t line_index) noexcept -> bool override
    {
      auto result_write_is_ok = bool { };

      if((pstr != nullptr) && (length > static_cast<std::uint_fast8_t>(UINT8_C(0))))
      {
        // Clear the line and reset the address to the line.
        clear_line    (static_cast<std::uint8_t>(line_index));
        set_line_index(static_cast<std::uint8_t>(line_index));

        // Write the line at line_index.
        for(auto   i = static_cast<std::size_t>(UINT8_C(0));
                   i < (std::min)(static_cast<std::size_t>(UINT8_C(16)), length);
                 ++i)
        {
          write_i(static_cast<std::uint8_t>(pstr[i]));
        }

        result_write_is_ok = true;
      }
      else
      {
        result_write_is_ok = false;
      }

      return result_write_is_ok;
    }

  private:
    static auto blocking_delay(const typename timer_type::tick_type dt) -> void
    {
      timer_type::blocking_delay(dt);
    }

    auto set_line_index(const std::uint8_t line_index) noexcept -> bool override
    {
      auto set_line_index_is_ok = bool { };

      if(line_index < static_cast<std::uint8_t>(UINT8_C(4)))
      {
        const auto command_byte =
          static_cast<std::uint8_t>
          (
              static_cast<std::uint8_t>(UINT8_C(0x80))
            | static_cast<std::uint8_t>
              (
                  static_cast<std::uint8_t>(line_index * static_cast<std::uint8_t>(UINT8_C(0x40)))
                + static_cast<std::uint8_t>
                  (
                    (line_index > static_cast<std::uint8_t>(UINT8_C(1)))
                      ? static_cast<std::uint8_t>(UINT8_C(20))
                      : static_cast<std::uint8_t>(UINT8_C(0))
                  )
              )
          );

        command(command_byte);

        set_line_index_is_ok = true;
      }
      else
      {
        set_line_index_is_ok = false;
      }

      return set_line_index_is_ok;
    }

    auto clear_line(const std::uint8_t line_index) noexcept -> void
    {
      static_cast<void>(set_line_index(line_index));

      // Clear the line at line_index.
      for(auto   i = static_cast<std::uint_fast8_t>(UINT8_C(0));
                 i < static_cast<std::uint_fast8_t>(UINT8_C(20));
               ++i)
      {
        write_i(static_cast<std::uint8_t>(' '));
      }

      static_cast<void>(set_line_index(line_index));
    }

    auto write_i(const std::uint8_t i) noexcept -> void
    {
      P1_set(i);                                                                      // P1 = i;   // Put data on the output Port
      port_pin_rs__type::set_pin_high();                                              // D_I =1;   // D/I=HIGH : send data
      port_pin_rw__type::set_pin_low();                                               // R_W =0;   // R/W=LOW : Write
      port_pin_e___type::set_pin_high();                                              // E = 1;
      blocking_delay(timer_type::microseconds(static_cast<unsigned>(UINT8_C(10))));   // Delay(1); //enable pulse width >= 300ns
      port_pin_e___type::set_pin_low();                                               // E = 0;    //Clock enable: falling edge
    }

    auto command(std::uint8_t i) noexcept -> void
    {
      P1_set(i);                                                                      // P1 = i;   //put data on output Port
      port_pin_rs__type::set_pin_low();                                               // D_I =0;   //D/I=LOW : send instruction
      port_pin_rw__type::set_pin_low();                                               // R_W =0;   //R/W=LOW : Write
      port_pin_e___type::set_pin_high();                                              // E = 1;
      blocking_delay(timer_type::microseconds(static_cast<unsigned>(UINT8_C(10))));   // Delay(1); //enable pulse width >= 300ns
      port_pin_e___type::set_pin_low();                                               // E = 0;    //Clock enable: falling edge
      blocking_delay(timer_type::microseconds(static_cast<unsigned>(UINT8_C(40))));
    }

    auto P1_set(const std::uint8_t c) noexcept -> void
    {
      static_cast<std::uint_fast8_t>(c & UINT8_C(0x01)) != UINT8_C(0) ? port_pin_db0_type::set_pin_high() : port_pin_db0_type::set_pin_low();
      static_cast<std::uint_fast8_t>(c & UINT8_C(0x02)) != UINT8_C(0) ? port_pin_db1_type::set_pin_high() : port_pin_db1_type::set_pin_low();
      static_cast<std::uint_fast8_t>(c & UINT8_C(0x04)) != UINT8_C(0) ? port_pin_db2_type::set_pin_high() : port_pin_db2_type::set_pin_low();
      static_cast<std::uint_fast8_t>(c & UINT8_C(0x08)) != UINT8_C(0) ? port_pin_db3_type::set_pin_high() : port_pin_db3_type::set_pin_low();
      static_cast<std::uint_fast8_t>(c & UINT8_C(0x10)) != UINT8_C(0) ? port_pin_db4_type::set_pin_high() : port_pin_db4_type::set_pin_low();
      static_cast<std::uint_fast8_t>(c & UINT8_C(0x20)) != UINT8_C(0) ? port_pin_db5_type::set_pin_high() : port_pin_db5_type::set_pin_low();
      static_cast<std::uint_fast8_t>(c & UINT8_C(0x40)) != UINT8_C(0) ? port_pin_db6_type::set_pin_high() : port_pin_db6_type::set_pin_low();
      static_cast<std::uint_fast8_t>(c & UINT8_C(0x80)) != UINT8_C(0) ? port_pin_db7_type::set_pin_high() : port_pin_db7_type::set_pin_low();
    }

    static auto P1_get(void) noexcept -> std::uint8_t
    {
      const std::uint8_t u =   static_cast<std::uint8_t>(static_cast<std::uint_fast8_t>(port_pin_db0_type::read_input_value() ? 1U : 0U) << 0U)
                             | static_cast<std::uint8_t>(static_cast<std::uint_fast8_t>(port_pin_db1_type::read_input_value() ? 1U : 0U) << 1U)
                             | static_cast<std::uint8_t>(static_cast<std::uint_fast8_t>(port_pin_db2_type::read_input_value() ? 1U : 0U) << 2U)
                             | static_cast<std::uint8_t>(static_cast<std::uint_fast8_t>(port_pin_db3_type::read_input_value() ? 1U : 0U) << 3U)
                             | static_cast<std::uint8_t>(static_cast<std::uint_fast8_t>(port_pin_db4_type::read_input_value() ? 1U : 0U) << 4U)
                             | static_cast<std::uint8_t>(static_cast<std::uint_fast8_t>(port_pin_db5_type::read_input_value() ? 1U : 0U) << 5U)
                             | static_cast<std::uint8_t>(static_cast<std::uint_fast8_t>(port_pin_db6_type::read_input_value() ? 1U : 0U) << 6U)
                             | static_cast<std::uint8_t>(static_cast<std::uint_fast8_t>(port_pin_db7_type::read_input_value() ? 1U : 0U) << 7U);

      return u;
    }

    static auto P1_set_direction_output() noexcept -> void
    {
      port_pin_db0_type::set_direction_output();
      port_pin_db1_type::set_direction_output();
      port_pin_db2_type::set_direction_output();
      port_pin_db3_type::set_direction_output();
      port_pin_db4_type::set_direction_output();
      port_pin_db5_type::set_direction_output();
      port_pin_db6_type::set_direction_output();
      port_pin_db7_type::set_direction_output();
    }

    static auto P1_set_direction_input() noexcept -> void
    {
      port_pin_db0_type::set_direction_input();
      port_pin_db1_type::set_direction_input();
      port_pin_db2_type::set_direction_input();
      port_pin_db3_type::set_direction_input();
      port_pin_db4_type::set_direction_input();
      port_pin_db5_type::set_direction_input();
      port_pin_db6_type::set_direction_input();
      port_pin_db7_type::set_direction_input();
    }
  };

  } } // namespace mcal::lcd

#endif // MCAL_LCD_ST7066U_NEWHAVEN_0216K1Z_2020_05_07_H_
