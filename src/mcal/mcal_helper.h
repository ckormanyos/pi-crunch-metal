///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_HELPER_2020_05_21_H
  #define MCAL_HELPER_2020_05_21_H

  #include <cstdint>
  #include <type_traits>

  #include <mcal_cpu.h>
  #include <mcal_irq.h>

  namespace mcal { namespace helper {

  constexpr auto nop_maker_switchover_to_loop = static_cast<std::uint_fast16_t>(UINT8_C(16));

  template<const std::uint_fast16_t nop_count>
  typename std::enable_if<(nop_count == static_cast<std::uint_fast16_t>(UINT8_C(0))), void>::type nop_maker() { }

  template<const std::uint_fast16_t nop_count>
  typename std::enable_if<(nop_count == static_cast<std::uint_fast16_t>(UINT8_C(1))), void>::type nop_maker() { mcal::cpu::nop(); }

  template<const std::uint_fast16_t nop_count>
  typename std::enable_if<(   (nop_count >  static_cast<std::uint_fast16_t>(UINT8_C(1)))
                           && (nop_count <= nop_maker_switchover_to_loop)), void>::type nop_maker()
  {
    nop_maker<static_cast<std::uint_fast16_t>(nop_count - static_cast<std::uint_fast16_t>(UINT8_C(1)))>();

    mcal::cpu::nop();
  }

  template<const std::uint_fast16_t nop_count>
  typename std::enable_if<(nop_count > nop_maker_switchover_to_loop), void>::type nop_maker()
  {
    for(auto i = static_cast<std::uint_fast16_t>(UINT8_C(0)); i < nop_count; ++i)
    {
      mcal::cpu::nop();
    }
  }

  template<const bool has_disable_enable_interrupts>
  void disable_all_interrupts(const bool = has_disable_enable_interrupts,
                              const typename std::enable_if<(has_disable_enable_interrupts == true)>::type* = nullptr) noexcept
  {
    mcal::irq::disable_all();
  }

  template<const bool has_disable_enable_interrupts>
  void enable_all_interrupts(const bool = has_disable_enable_interrupts,
                             const typename std::enable_if<(has_disable_enable_interrupts == true)>::type* = nullptr) noexcept
  {
    mcal::irq::enable_all();
  }

  template<const bool has_disable_enable_interrupts>
  void disable_all_interrupts(const bool = has_disable_enable_interrupts,
                              const typename std::enable_if<(has_disable_enable_interrupts == false)>::type* = nullptr) noexcept { }

  template<const bool has_disable_enable_interrupts>
  void enable_all_interrupts(const bool = has_disable_enable_interrupts,
                             const typename std::enable_if<(has_disable_enable_interrupts == false)>::type* = nullptr) noexcept { }

  } } // namespace mcal::helper

#endif // MCAL_HELPER_2020_05_21_H
