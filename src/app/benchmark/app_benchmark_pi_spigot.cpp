///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022.                               //
//  Copyright Christopher Kormanyos 2019 - 2023.                 //
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

// cd /mnt/c/Users/User/Documents/Ks/PC_Software/NumericalPrograms/pi_spigot
// g++ -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Winit-self -O3 -std=c++17 -I. -finline-functions pi_spigot.cpp pi_spigot/pi_spigot_ctrl.cpp -o pi_spigot.exe

#include <math/checksums/hash/hash_sha1.h>
#include <mcal_lcd.h>
#include <math/pi_spigot/pi_spigot.h>
#include <util/utility/util_baselexical_cast.h>

//#define PI_SPIGOT_USE_10K_DIGITS

namespace app { namespace benchmark {

bool run_pi_calc();

namespace detail {

void app_benchmark_callback_for_pi_digits10(const std::uint32_t d10);

std::uint32_t app_benchmark_pi_count;

} // namespace app::benchmark::local

} } // namespace app::benchmark

void app::benchmark::detail::app_benchmark_callback_for_pi_digits10(const std::uint32_t d10)
{
  char pstr[10U] = { 0 };

  const char* pend = util::baselexical_cast(d10, pstr);

  mcal::lcd::lcd0().write(pstr, static_cast<std::uint_fast8_t>(pend - pstr), 0U);

  std::fill(pstr, pstr + sizeof(pstr), (char) 0);

  pend = util::baselexical_cast(app_benchmark_pi_count, pstr);

  mcal::lcd::lcd0().write(pstr, static_cast<std::uint_fast8_t>(pend - pstr), 1U);
}

namespace local
{
  #if defined(PI_SPIGOT_USE_10K_DIGITS)
  constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(10001));
  #else
  constexpr auto result_digit = static_cast<std::uint32_t>(UINT32_C(100001));
  #endif

  constexpr auto loop_digit = static_cast<std::uint32_t>(UINT8_C(9));

  using pi_spigot_type = math::constants::pi_spigot<result_digit, loop_digit>;

  pi_spigot_type pi_spigot_instance;

  using hash_type = math::checksums::hash::hash_sha1;

  hash_type pi_spigot_hash;

  auto pi_spigot_count_of_calculations = static_cast<std::uint32_t>(UINT8_C(0));
}

auto app::benchmark::run_pi_calc() -> bool
{
  local::pi_spigot_instance.calculate(detail::app_benchmark_callback_for_pi_digits10, &local::pi_spigot_hash);

  // Check the hash result of the pi calculation.
  const auto hash_control =
    typename local::hash_type::result_type
    #if defined(PI_SPIGOT_USE_10K_DIGITS)
    {
      // 10001: 4BDF69A5FF25B9BED6BA9802BD2A68306FAB71EC
      0x4BU, 0xDFU, 0x69U, 0xA5U, 0xFFU, 0x25U, 0xB9U, 0xBEU,
      0xD6U, 0xBAU, 0x98U, 0x02U, 0xBDU, 0x2AU, 0x68U, 0x30U,
      0x6FU, 0xABU, 0x71U, 0xECU
    };
    #else
    {
      // 100001: D9D56240EB6B626A8FE179E3054D332F1767071D
      0xD9U, 0xD5U, 0x62U, 0x40U, 0xEBU, 0x6BU, 0x62U, 0x6AU,
      0x8FU, 0xE1U, 0x79U, 0xE3U, 0x05U, 0x4DU, 0x33U, 0x2FU,
      0x17U, 0x67U, 0x07U, 0x1DU
    };
    #endif

  auto hash_result = typename local::hash_type::result_type { };

  local::pi_spigot_hash.get_result(hash_result.data());

  const auto result_pi_spigot_is_ok =
    std::equal(hash_result.cbegin(), hash_result.cend(), hash_control.cbegin());

  ++app::benchmark::detail::app_benchmark_pi_count;

  return result_pi_spigot_is_ok;
}
