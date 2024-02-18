///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022.                               //
//  Copyright Christopher Kormanyos 2019 - 2024.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

// pi_spigot.cpp

// This program can be used to compute many thousands
// of decimal digits of digits of pi. Although it uses
// a so-called spigot algorithm having quadratic complexity,
// this program can, albeit slowly, compute up to a few million
// decimal digits of pi on a PC host. The test data provided
// below, however, only allow for testing up to about 100,000
// decimal digits.

// cd /mnt/c/Users/User/Documents/Ks/uC_Software/Boards/pi-crunch-metal
// g++ -Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wundef -Wunused-parameter -Wuninitialized -Wunreachable-code -Winit-self -Wzero-as-null-pointer-constant -O3 -std=c++17 -finline-functions -finline-limit=64 -Isrc -Isrc/mcal/host -DPI_CRUNCH_METAL_STANDALONE_MAIN -DPI_CRUNCH_METAL_CFG_METHOD=PI_CRUNCH_METAL_CFG_USE_PI_SPIGOT ./src/app/Appli/pi_agm/pi_agm.cpp ./src/app/Appli/pi_spigot/pi_spigot.cpp ./src/mcal/host/mcal_cpu.cpp ./src/mcal/host/mcal_gpt.cpp ./src/mcal/host/mcal_irq.cpp ./src/mcal/host/mcal_lcd.cpp ./src/mcal/host/mcal_led.cpp ./src/mcal/host/mcal_port.cpp ./src/mcal/host/mcal_spi.cpp ./src/mcal/mcal.cpp -o pi-crunch-metal

#include <app/Appli/pi_calc_cfg.h>

#if defined(PI_CRUNCH_METAL_STANDALONE_MAIN)
#include <iomanip>
#include <iostream>
#endif // PI_CRUNCH_METAL_STANDALONE_MAIN

#if (defined(PI_CRUNCH_METAL_CFG_METHOD) && (PI_CRUNCH_METAL_CFG_METHOD == PI_CRUNCH_METAL_CFG_USE_PI_SPIGOT))

#include <math/checksums/hash/hash_sha1.h>
#include <mcal_lcd.h>
#include <math/pi_spigot/pi_spigot.h>
#include <util/utility/util_baselexical_cast.h>

namespace local
{
  #if defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_100_DIGITS)
  constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(101));
  #elif defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_1K_DIGITS)
  constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(1001));
  #elif defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_10K_DIGITS)
  constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(10001));
  #elif defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_100K_DIGITS) || (!defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_100_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_1K_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_10K_DIGITS))
  constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(100001));
  #else
  #error Error: Wrong pi_spigot digit configuration
  #endif

  constexpr auto loop_digit = static_cast<std::uint32_t>(UINT8_C(9));

  using pi_spigot_type = math::constants::pi_spigot<result_digit, loop_digit>;

  pi_spigot_type pi_spigot_instance;

  using hash_type = math::checksums::hash::hash_sha1;

  hash_type pi_spigot_hash;

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
  char p_str[static_cast<std::size_t>(UINT8_C(10))] =
  {
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'
  };

  const char* pend = util::baselexical_cast(local::pi_output_digits10, p_str, p_str + sizeof(p_str));

  mcal::lcd::lcd0().write(pstr, static_cast<std::uint_fast8_t>(pend - pstr), 0U);

  std::fill(pstr, pstr + sizeof(pstr), (char) 0);

  pend = util::baselexical_cast(local::pi_count_of_calculations, pstr, p_str + sizeof(p_str));

  mcal::lcd::lcd0().write(pstr, static_cast<std::uint_fast8_t>(pend - pstr), 1U);
}

auto pi_main() -> int
{
  local::pi_spigot_instance.calculate(local::pi_output_count_write, &local::pi_spigot_hash);

  // Check the hash result of the pi calculation.
  const auto hash_control =
    typename local::hash_type::result_type
    #if defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_100_DIGITS)
    {
      0x93U, 0xF1U, 0xB4U, 0xEAU, 0xABU, 0xCBU, 0xC9U, 0xB9U,
      0x0CU, 0x93U, 0x93U, 0x24U, 0xF7U, 0x85U, 0x13U, 0x2EU,
      0xDFU, 0x3BU, 0xF2U, 0x01U
    };
    #elif defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_1K_DIGITS)
    {
      0xA0U, 0x92U, 0x47U, 0x1FU, 0xD5U, 0xFEU, 0x41U, 0x51U,
      0x20U, 0xE7U, 0xDDU, 0x18U, 0x5BU, 0x93U, 0x0DU, 0x05U,
      0x3AU, 0x86U, 0xF1U, 0x7EU
    };
    #elif defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_10K_DIGITS)
    {
      // 10001: 4BDF69A5FF25B9BED6BA9802BD2A68306FAB71EC
      0x4BU, 0xDFU, 0x69U, 0xA5U, 0xFFU, 0x25U, 0xB9U, 0xBEU,
      0xD6U, 0xBAU, 0x98U, 0x02U, 0xBDU, 0x2AU, 0x68U, 0x30U,
      0x6FU, 0xABU, 0x71U, 0xECU
    };
    #elif defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_100K_DIGITS) || (!defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_100_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_1K_DIGITS) && !defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_10K_DIGITS))
    {
      // 100001: D9D56240EB6B626A8FE179E3054D332F1767071D
      0xD9U, 0xD5U, 0x62U, 0x40U, 0xEBU, 0x6BU, 0x62U, 0x6AU,
      0x8FU, 0xE1U, 0x79U, 0xE3U, 0x05U, 0x4DU, 0x33U, 0x2FU,
      0x17U, 0x67U, 0x07U, 0x1DU
    };
    #else
    #error Error: Wrong pi_spigot digit configuration
    #endif

  auto hash_result = typename local::hash_type::result_type { };

  local::pi_spigot_hash.get_result(hash_result.data());

  const auto result_is_ok =
    std::equal(hash_result.cbegin(), hash_result.cend(), hash_control.cbegin());

  ++local::pi_count_of_calculations;

  return (result_is_ok ? 0 : -1);
}

#if defined(PI_CRUNCH_METAL_STANDALONE_MAIN)

extern "C"
auto mcal_init(void) -> void;

auto main(void) -> int
{
  ::mcal_init();

  std::cout << "Begin pi spigot calculation..." << std::endl;

  const auto result_pi_main = ::pi_main();

  const auto result_is_ok = (result_pi_main == 0);

  {
    const auto flg = std::cout.flags();

    std::cout << "digits10:     "                   << local::pi_spigot_type::result_digit() << '\n';
    std::cout << "result_is_ok: " << std::boolalpha << result_is_ok;

    std::cout.flags(flg);
  }

  return (result_is_ok ? 0 : -1);
}

#endif // PI_CRUNCH_METAL_STANDALONE_MAIN

#else

auto pi_calc_dummy() -> void;

auto pi_calc_dummy() -> void { }

#endif
