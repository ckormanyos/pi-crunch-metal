///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#if defined(_MSC_VER) && !defined(PI_CRUNCH_METAL_STANDALONE_MAIN)
#define PI_CRUNCH_METAL_STANDALONE_MAIN
#endif

#include <algorithm>
#include <cmath>
#include <cstdint>
#if defined(PI_CRUNCH_METAL_STANDALONE_MAIN)
#include <iomanip>
#include <iostream>
#endif // PI_CRUNCH_METAL_STANDALONE_MAIN

#include <mcal_lcd.h>
#include <util/utility/util_baselexical_cast.h>

#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS

#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

namespace math { namespace constants {

constexpr std::array<std::uint32_t, 7U + 1U> const_pi_control_head_32
{
  // head std::uint32_t
  (std::uint32_t) UINT32_C(3),
  (std::uint32_t) UINT32_C(14159265),
  (std::uint32_t) UINT32_C(35897932),
  (std::uint32_t) UINT32_C(38462643),
  (std::uint32_t) UINT32_C(38327950),
  (std::uint32_t) UINT32_C(28841971),
  (std::uint32_t) UINT32_C(69399375),
  (std::uint32_t) UINT32_C(10582097)
};

constexpr std::array<std::uint32_t, 8U> const_pi_control_tail_32_1000001
{
  // tail 1 + 10^6 std::uint32_t
  (std::uint32_t) UINT32_C(20875424),
  (std::uint32_t) UINT32_C(50598956),
  (std::uint32_t) UINT32_C(78796130),
  (std::uint32_t) UINT32_C(33116462),
  (std::uint32_t) UINT32_C(83996346),
  (std::uint32_t) UINT32_C(46042209),
  (std::uint32_t) UINT32_C( 1061057),
  (std::uint32_t) UINT32_C(79458151)
};

} } //   namespace math::constants

namespace local
{
  auto pi_count_of_calculations = static_cast<std::uint32_t>(UINT8_C(0));

  auto pi_output_digits10 = static_cast<std::uint32_t>(UINT8_C(0));

  auto pi_output_count_write(const std::uint32_t d10) -> void;

  auto pi_output_count_write(const std::uint32_t d10) -> void { local::pi_output_digits10 = d10; }
} // namespace local

extern "C"
{
  auto mcal_led_toggle(void) -> void;

  auto pi_main() -> int;

  auto pi_led_toggle(void) -> void;

  auto pi_lcd_progress(void) -> void;
}

extern "C"
auto pi_led_toggle(void) -> void
{
  ::mcal_led_toggle();
}

extern "C"
auto pi_lcd_progress(void) -> void
{
  char pstr[10U] = { 0 };

  const char* pend = util::baselexical_cast(local::pi_output_digits10, pstr);

  mcal::lcd::lcd0().write(pstr, static_cast<std::uint_fast8_t>(pend - pstr), 0U);

  std::fill(pstr, pstr + sizeof(pstr), (char) 0);

  pend = util::baselexical_cast(local::pi_count_of_calculations, pstr);

  mcal::lcd::lcd0().write(pstr, static_cast<std::uint_fast8_t>(pend - pstr), 1U);
}

auto pi_main() -> int
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1000001);

  constexpr std::int32_t local_elem_number =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

  using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 16U>;

  using local_wide_decimal_type =
    math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, double, std::int32_t, double>;

  const local_wide_decimal_type my_pi =
    math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, double, std::int32_t, double>
    (
      local::pi_output_count_write
    );

  auto result_is_ok = true;

  {
    const bool head_is_ok =
      std::equal
      (
        my_pi.crepresentation().cbegin(),
        my_pi.crepresentation().cbegin() + math::constants::const_pi_control_head_32.size(),
        math::constants::const_pi_control_head_32.begin()
      );

    result_is_ok &= head_is_ok;
  }

  {
    constexpr std::int32_t local_elem_digits10 =
      math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;

    using local_const_iterator_type = typename local_wide_decimal_type::representation_type::const_iterator;
    using local_difference_type     = typename local_wide_decimal_type::representation_type::difference_type;

    const local_difference_type distance_to_ctrl =
      static_cast<local_difference_type>
      (
          static_cast<local_difference_type>(1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10))
        - static_cast<local_difference_type>(math::constants::const_pi_control_tail_32_1000001.size())
      );

    local_const_iterator_type fwd_iterator_ctrl = my_pi.crepresentation().cbegin() + distance_to_ctrl;

    const bool tail_is_ok =
      std::equal
      (
        fwd_iterator_ctrl,
        fwd_iterator_ctrl + math::constants::const_pi_control_tail_32_1000001.size(),
        math::constants::const_pi_control_tail_32_1000001.begin()
      );

    result_is_ok &= tail_is_ok;
  }

  ++local::pi_count_of_calculations;

  return (result_is_ok ? 0 : 1);
}

#if defined(PI_CRUNCH_METAL_STANDALONE_MAIN)

extern "C"
auto mcal_init(void) -> void;

auto main(void) -> int
{
  ::mcal_init();

  std::cout << "Begin pi AGM calculation..." << std::endl;

  const auto result_pi_main = ::pi_main();

  const auto result_is_ok = (result_pi_main == 0);

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;

  return (result_is_ok ? 0 : -1);
}

#endif // PI_CRUNCH_METAL_STANDALONE_MAIN
