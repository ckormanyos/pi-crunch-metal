﻿///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021 - 2024.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#if (defined(__GNUC__) && (__GNUC__ >= 12))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overread"
#endif

// cd /mnt/c/Users/User/Documents/Ks/uC_Software/Boards/pi-crunch-metal
// g++ -Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wundef -Wunused-parameter -Wuninitialized -Wunreachable-code -Winit-self -Wzero-as-null-pointer-constant -O3 -std=c++20 -finline-functions -finline-limit=64 -Isrc -Isrc/mcal/host -DPI_CRUNCH_METAL_STANDALONE_MAIN -DPI_CRUNCH_METAL_CFG_METHOD=PI_CRUNCH_METAL_CFG_USE_PI_AGM ./src/app/Appli/pi_agm/pi_agm.cpp ./src/app/Appli/pi_spigot/pi_spigot.cpp ./src/mcal/host/mcal_cpu.cpp ./src/mcal/host/mcal_gpt.cpp ./src/mcal/host/mcal_irq.cpp ./src/mcal/host/mcal_lcd.cpp ./src/mcal/host/mcal_led.cpp ./src/mcal/host/mcal_port.cpp ./src/mcal/host/mcal_spi.cpp ./src/mcal/mcal.cpp -o pi-crunch-metal

#include <app/Appli/pi_calc_cfg.h>

#if (defined(PI_CRUNCH_METAL_CFG_METHOD) && (PI_CRUNCH_METAL_CFG_METHOD == PI_CRUNCH_METAL_CFG_USE_PI_AGM))

#include <mcal_lcd.h>
#include <util/utility/util_baselexical_cast.h>

#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_USE_STD_FUNCTION

#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>

#if defined(PI_CRUNCH_METAL_STANDALONE_MAIN)
#include <iomanip>
#include <iostream>
#endif // PI_CRUNCH_METAL_STANDALONE_MAIN
#include <span>

namespace math { namespace constants {

using pi_control_array_type = std::array<std::uint32_t, static_cast<std::size_t>(UINT8_C(8))>;

constexpr auto pi_control_head =
  pi_control_array_type
  {
    // head std::uint32_t
    std::uint32_t { UINT32_C(3) },
    std::uint32_t { UINT32_C(14159265) },
    std::uint32_t { UINT32_C(35897932) },
    std::uint32_t { UINT32_C(38462643) },
    std::uint32_t { UINT32_C(38327950) },
    std::uint32_t { UINT32_C(28841971) },
    std::uint32_t { UINT32_C(69399375) },
    std::uint32_t { UINT32_C(10582097) }
  };

#if defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_100_DIGITS)

constexpr auto pi_control_tail =
  pi_control_array_type
  {
    // tail 1 + 10^2 std::uint32_t
    std::uint32_t { UINT32_C(28841971) },
    std::uint32_t { UINT32_C(69399375) },
    std::uint32_t { UINT32_C(10582097) },
    std::uint32_t { UINT32_C(49445923) },
    std::uint32_t { UINT32_C( 7816406) },
    std::uint32_t { UINT32_C(28620899) },
    std::uint32_t { UINT32_C(86280348) },
    std::uint32_t { UINT32_C(25342117) }
  };

#elif defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_1K_DIGITS)

constexpr auto pi_control_tail =
  pi_control_array_type
  {
    // tail 1 + 10^3 std::uint32_t
    std::uint32_t { UINT32_C(78759375) },
    std::uint32_t { UINT32_C(19577818) },
    std::uint32_t { UINT32_C(57780532) },
    std::uint32_t { UINT32_C(17122680) },
    std::uint32_t { UINT32_C(66130019) },
    std::uint32_t { UINT32_C(27876611) },
    std::uint32_t { UINT32_C(19590921) },
    std::uint32_t { UINT32_C(64201989) }
  };

#elif defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_10K_DIGITS)

constexpr auto pi_control_tail =
  pi_control_array_type
  {
    // tail 1 + 10^4 std::uint32_t
    std::uint32_t { UINT32_C(29552498) },
    std::uint32_t { UINT32_C(87275846) },
    std::uint32_t { UINT32_C(10126483) },
    std::uint32_t { UINT32_C(69998922) },
    std::uint32_t { UINT32_C(56959688) },
    std::uint32_t { UINT32_C(15920560) },
    std::uint32_t { UINT32_C( 1016552) },
    std::uint32_t { UINT32_C(56375678) }
  };

#elif defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_100K_DIGITS)

constexpr auto pi_control_tail =
  pi_control_array_type
  {
    // tail 1 + 10^5 std::uint32_t
    std::uint32_t { UINT32_C(38043299) },
    std::uint32_t { UINT32_C(70695770) },
    std::uint32_t { UINT32_C(15078933) },
    std::uint32_t { UINT32_C(77286580) },
    std::uint32_t { UINT32_C(35712790) },
    std::uint32_t { UINT32_C(91376742) },
    std::uint32_t { UINT32_C( 8056554) },
    std::uint32_t { UINT32_C(93624646) }
  };

#elif defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_1M_DIGITS) || (!defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_100_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_1K_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_10K_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_100K_DIGITS))

constexpr auto pi_control_tail =
  pi_control_array_type
  {
    // tail 1 + 10^6 std::uint32_t
    std::uint32_t { UINT32_C(20875424) },
    std::uint32_t { UINT32_C(50598956) },
    std::uint32_t { UINT32_C(78796130) },
    std::uint32_t { UINT32_C(33116462) },
    std::uint32_t { UINT32_C(83996346) },
    std::uint32_t { UINT32_C(46042209) },
    std::uint32_t { UINT32_C( 1061057) },
    std::uint32_t { UINT32_C(79458151) }
  };

#else

#error Error: Wrong pi_agm digit configuration

#endif

} } //   namespace math::constants

namespace local
{
  std::uint32_t pi_count_of_calculations { std::uint32_t { UINT8_C(0) } };

  std::uint32_t pi_output_digits10 = { std::uint32_t { UINT8_C(0) } };

  auto pi_output_count_write(const std::uint32_t d10) noexcept -> void;

  auto pi_output_count_write(const std::uint32_t d10) noexcept -> void { local::pi_output_digits10 = d10; }
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
  char p_str[static_cast<std::size_t>(UINT8_C(10))] =
  {
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
  };

  std::span<char> str_span { p_str, sizeof(p_str) };

  {
    const char* p_end { util::baselexical_cast(local::pi_output_digits10, str_span.data(), str_span.data() + str_span.size()) };

    const std::size_t len { static_cast<std::size_t>(std::distance(static_cast<const char*>(str_span.data()), p_end)) };

    mcal::lcd::lcd0().write(str_span.data(), len, std::uint_fast8_t { UINT8_C(0) });
  }

  std::fill(str_span.data(), str_span.data() + str_span.size(), char { INT8_C(0) });

  {
    const char* p_end { util::baselexical_cast(local::pi_count_of_calculations, str_span.data(), str_span.data() + str_span.size()) };

    const std::size_t len { static_cast<std::size_t>(std::distance(static_cast<const char*>(str_span.data()), p_end)) };

    mcal::lcd::lcd0().write(str_span.data(), len, std::uint_fast8_t { UINT8_C(1) });
  }
}

namespace local
{
  using limb_type = std::uint32_t;

  #if defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_100_DIGITS)
  constexpr auto wide_decimal_digits10 = std::int32_t { INT32_C(101) };
  #elif defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_1K_DIGITS)
  constexpr auto wide_decimal_digits10 = std::int32_t { INT32_C(1001) };
  #elif defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_10K_DIGITS)
  constexpr auto wide_decimal_digits10 = std::int32_t { INT32_C(10001) };
  #elif defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_100K_DIGITS)
  constexpr auto wide_decimal_digits10 = std::int32_t { INT32_C(100001) };
  #elif defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_1M_DIGITS) || (!defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_100_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_1K_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_10K_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_100K_DIGITS))
  constexpr auto wide_decimal_digits10 = std::int32_t { INT32_C(1000001) };
  #else
  #error Error: Wrong pi_agm digit configuration
  #endif

  constexpr auto elem_number =
    ::math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local::limb_type>::elem_number;

  using allocator_type =
    util::n_slot_array_allocator<void, local::elem_number, std::size_t { UINT8_C(16) }>;

  using wide_decimal_type =
    ::math::wide_decimal::decwide_t<wide_decimal_digits10, local::limb_type, local::allocator_type, double, std::int32_t, double>;
}

auto pi_main() -> int
{
  const local::wide_decimal_type my_pi =
    ::math::wide_decimal::pi<local::wide_decimal_digits10, local::limb_type, local::allocator_type, double, std::int32_t, double>
    (
      local::pi_output_count_write
    );

  auto result_is_ok = true;

  using local_difference_type = typename local::wide_decimal_type::representation_type::difference_type;

  {
    const bool head_is_ok =
      std::equal
      (
        my_pi.crepresentation().cbegin(),
        my_pi.crepresentation().cbegin() + std::tuple_size<::math::constants::pi_control_array_type>::value,
        ::math::constants::pi_control_head.cbegin()
      );

    result_is_ok = (head_is_ok && result_is_ok);
  }

  {
    constexpr auto local_elem_digits10 =
      ::math::wide_decimal::detail::decwide_t_helper<local::wide_decimal_digits10, local::limb_type>::elem_digits10;

    const auto distance_to_ctrl =
      static_cast<local_difference_type>
      (
          static_cast<std::int32_t>
          (
              static_cast<std::int32_t>(INT8_C(1))
            + static_cast<std::int32_t>
              (
                static_cast<std::int32_t>
                (
                  local::wide_decimal_digits10 - static_cast<std::int32_t>(INT8_C(1))
                )
                / local_elem_digits10
              )
          )
        - static_cast<std::int32_t>(std::tuple_size<::math::constants::pi_control_array_type>::value)
      );

    auto fwd_iterator_ctrl = my_pi.crepresentation().cbegin() + distance_to_ctrl;

    const bool tail_is_ok =
      std::equal
      (
        fwd_iterator_ctrl,
        fwd_iterator_ctrl + std::tuple_size<::math::constants::pi_control_array_type>::value,
        ::math::constants::pi_control_tail.cbegin()
      );

    result_is_ok = (tail_is_ok && result_is_ok);
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

  {
    const auto flg = std::cout.flags();

    std::cout << "digits10:     "                   << std::numeric_limits<local::wide_decimal_type>::digits10 << '\n';
    std::cout << "result_is_ok: " << std::boolalpha << result_is_ok;
    std::cout << std::endl;

    std::cout.flags(flg);
  }

  return (result_is_ok ? 0 : -1);
}

#endif // PI_CRUNCH_METAL_STANDALONE_MAIN

#else

auto pi_calc_dummy() -> void;

auto pi_calc_dummy() -> void { }

#endif

#if (defined(__GNUC__) && (__GNUC__ >= 12))
#pragma GCC diagnostic pop
#pragma GCC diagnostic pop
#endif
