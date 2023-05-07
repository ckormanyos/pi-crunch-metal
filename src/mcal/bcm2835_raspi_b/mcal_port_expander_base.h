///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_PORT_EXPANDER_BASE_2023_05_05_H
  #define MCAL_PORT_EXPANDER_BASE_2023_05_05_H

  #include <cstdint>

  #include <util/utility/util_noncopyable.h>

  namespace mcal { namespace port {

  class port_expander_base : private util::noncopyable
  {
  public:
    virtual ~port_expander_base() = default;

    virtual auto read__word(const std::uint8_t)                      noexcept -> std::uint16_t = 0;
    virtual auto write_word(const std::uint8_t, const std::uint16_t) noexcept -> void          = 0;

    virtual auto set_direction_output(const std::uint8_t bpos) noexcept -> void = 0;
    virtual auto set_direction_input (const std::uint8_t bpos) noexcept -> void = 0;
    virtual auto set_pin_high        (const std::uint8_t bpos) noexcept -> void = 0;
    virtual auto set_pin_low         (const std::uint8_t bpos) noexcept -> void = 0;
    virtual auto toggle_pin          (const std::uint8_t bpos) noexcept -> void = 0;

  protected:
    port_expander_base() = default;
  };

  } } // namespace mcal::port

#endif // MCAL_PORT_EXPANDER_BASE_2023_05_05_H
