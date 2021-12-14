///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <algorithm>
#include <cmath>
#include <cstdint>

#include <mcal_lcd.h>
#include <util/utility/util_baselexical_cast.h>

namespace app { namespace benchmark {

bool run_pi_agm();

} } // namespace app::benchmark

#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING
#define WIDE_DECIMAL_DISABLE_CACHED_CONSTANTS

#include <math/wide_decimal/decwide_t.h>
#include <mcal_memory/mcal_memory_progmem_array.h>
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

constexpr std::array<std::uint16_t, 14U + 1U> const_pi_control_head_16
{
  // head std::uint16_t
  (std::uint16_t) UINT16_C(3),
  (std::uint16_t) UINT16_C(1415), (std::uint16_t) UINT16_C(9265),
  (std::uint16_t) UINT16_C(3589), (std::uint16_t) UINT16_C(7932),
  (std::uint16_t) UINT16_C(3846), (std::uint16_t) UINT16_C(2643),
  (std::uint16_t) UINT16_C(3832), (std::uint16_t) UINT16_C(7950),
  (std::uint16_t) UINT16_C(2884), (std::uint16_t) UINT16_C(1971),
  (std::uint16_t) UINT16_C(6939), (std::uint16_t) UINT16_C(9375),
  (std::uint16_t) UINT16_C(1058), (std::uint16_t) UINT16_C(2097)
};

constexpr std::array<std::uint8_t, 28U + 1U> const_pi_control_head_08
{
  // head std::uint8_t
  (std::uint8_t) UINT8_C(3),
  (std::uint8_t) UINT8_C(14), (std::uint8_t) UINT8_C(15), (std::uint8_t) UINT8_C(92), (std::uint8_t) UINT8_C(65),
  (std::uint8_t) UINT8_C(35), (std::uint8_t) UINT8_C(89), (std::uint8_t) UINT8_C(79), (std::uint8_t) UINT8_C(32),
  (std::uint8_t) UINT8_C(38), (std::uint8_t) UINT8_C(46), (std::uint8_t) UINT8_C(26), (std::uint8_t) UINT8_C(43),
  (std::uint8_t) UINT8_C(38), (std::uint8_t) UINT8_C(32), (std::uint8_t) UINT8_C(79), (std::uint8_t) UINT8_C(50),
  (std::uint8_t) UINT8_C(28), (std::uint8_t) UINT8_C(84), (std::uint8_t) UINT8_C(19), (std::uint8_t) UINT8_C(71),
  (std::uint8_t) UINT8_C(69), (std::uint8_t) UINT8_C(39), (std::uint8_t) UINT8_C(93), (std::uint8_t) UINT8_C(75),
  (std::uint8_t) UINT8_C(10), (std::uint8_t) UINT8_C(58), (std::uint8_t) UINT8_C(20), (std::uint8_t) UINT8_C(97)
};

constexpr std::array<std::uint32_t, 8U> const_pi_control_tail_32_10001
{
  // tail 1 + 10^4 std::uint32_t
  (std::uint32_t) UINT32_C(29552498),
  (std::uint32_t) UINT32_C(87275846),
  (std::uint32_t) UINT32_C(10126483),
  (std::uint32_t) UINT32_C(69998922),
  (std::uint32_t) UINT32_C(56959688),
  (std::uint32_t) UINT32_C(15920560),
  (std::uint32_t) UINT32_C( 1016552),
  (std::uint32_t) UINT32_C(56375678)
};

constexpr std::array<std::uint32_t, 8U> const_pi_control_tail_32_100001
{
  // tail 1 + 10^5 std::uint32_t
  (std::uint32_t) UINT32_C(38043299),
  (std::uint32_t) UINT32_C(70695770),
  (std::uint32_t) UINT32_C(15078933),
  (std::uint32_t) UINT32_C(77286580),
  (std::uint32_t) UINT32_C(35712790),
  (std::uint32_t) UINT32_C(91376742),
  (std::uint32_t) UINT32_C( 8056554),
  (std::uint32_t) UINT32_C(93624646)
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

constexpr std::array<std::uint16_t, 16U> const_pi_control_tail_16_10001
{
  // tail 1 + 10^4 std::uint16_t
  (std::uint16_t) UINT16_C(2955), (std::uint16_t) UINT16_C(2498),
  (std::uint16_t) UINT16_C(8727), (std::uint16_t) UINT16_C(5846),
  (std::uint16_t) UINT16_C(1012), (std::uint16_t) UINT16_C(6483),
  (std::uint16_t) UINT16_C(6999), (std::uint16_t) UINT16_C(8922),
  (std::uint16_t) UINT16_C(5695), (std::uint16_t) UINT16_C(9688),
  (std::uint16_t) UINT16_C(1592), (std::uint16_t) UINT16_C( 560),
  (std::uint16_t) UINT16_C( 101), (std::uint16_t) UINT16_C(6552),
  (std::uint16_t) UINT16_C(5637), (std::uint16_t) UINT16_C(5678)
};

constexpr std::array<std::uint16_t, 16U> const_pi_control_tail_16_100001
{
  // tail 1 + 10^5 std::uint16_t
  (std::uint16_t) UINT16_C(3804), (std::uint16_t) UINT16_C(3299),
  (std::uint16_t) UINT16_C(7069), (std::uint16_t) UINT16_C(5770),
  (std::uint16_t) UINT16_C(1507), (std::uint16_t) UINT16_C(8933),
  (std::uint16_t) UINT16_C(7728), (std::uint16_t) UINT16_C(6580),
  (std::uint16_t) UINT16_C(3571), (std::uint16_t) UINT16_C(2790),
  (std::uint16_t) UINT16_C(9137), (std::uint16_t) UINT16_C(6742),
  (std::uint16_t) UINT16_C( 805), (std::uint16_t) UINT16_C(6554),
  (std::uint16_t) UINT16_C(9362), (std::uint16_t) UINT16_C(4646)
};

constexpr std::array<std::uint16_t, 16U> const_pi_control_tail_16_1000001
{
  // tail 1 + 10^6 std::uint16_t
  (std::uint16_t) UINT16_C(2087), (std::uint16_t) UINT16_C(5424),
  (std::uint16_t) UINT16_C(5059), (std::uint16_t) UINT16_C(8956),
  (std::uint16_t) UINT16_C(7879), (std::uint16_t) UINT16_C(6130),
  (std::uint16_t) UINT16_C(3311), (std::uint16_t) UINT16_C(6462),
  (std::uint16_t) UINT16_C(8399), (std::uint16_t) UINT16_C(6346),
  (std::uint16_t) UINT16_C(4604), (std::uint16_t) UINT16_C(2209),
  (std::uint16_t) UINT16_C( 106), (std::uint16_t) UINT16_C(1057),
  (std::uint16_t) UINT16_C(7945), (std::uint16_t) UINT16_C(8151)
};

constexpr std::array<std::uint8_t, 32U> const_pi_control_tail_08_10001
{
  // tail 1 + 10^4 std::uint8_t
  (std::uint8_t) UINT8_C(29), (std::uint8_t) UINT8_C(55), (std::uint8_t) UINT8_C(24), (std::uint8_t) UINT8_C(98),
  (std::uint8_t) UINT8_C(87), (std::uint8_t) UINT8_C(27), (std::uint8_t) UINT8_C(58), (std::uint8_t) UINT8_C(46),
  (std::uint8_t) UINT8_C(10), (std::uint8_t) UINT8_C(12), (std::uint8_t) UINT8_C(64), (std::uint8_t) UINT8_C(83),
  (std::uint8_t) UINT8_C(69), (std::uint8_t) UINT8_C(99), (std::uint8_t) UINT8_C(89), (std::uint8_t) UINT8_C(22),
  (std::uint8_t) UINT8_C(56), (std::uint8_t) UINT8_C(95), (std::uint8_t) UINT8_C(96), (std::uint8_t) UINT8_C(88),
  (std::uint8_t) UINT8_C(15), (std::uint8_t) UINT8_C(92), (std::uint8_t) UINT8_C( 5), (std::uint8_t) UINT8_C(60),
  (std::uint8_t) UINT8_C( 1), (std::uint8_t) UINT8_C( 1), (std::uint8_t) UINT8_C(65), (std::uint8_t) UINT8_C(52),
  (std::uint8_t) UINT8_C(56), (std::uint8_t) UINT8_C(37), (std::uint8_t) UINT8_C(56), (std::uint8_t) UINT8_C(78)
};

constexpr std::array<std::uint8_t, 32U> const_pi_control_tail_08_100001
{
  // tail 1 + 10^5 std::uint8_t
  (std::uint8_t) UINT8_C(38), (std::uint8_t) UINT8_C( 4), (std::uint8_t) UINT8_C(32), (std::uint8_t) UINT8_C(99),
  (std::uint8_t) UINT8_C(70), (std::uint8_t) UINT8_C(69), (std::uint8_t) UINT8_C(57), (std::uint8_t) UINT8_C(70),
  (std::uint8_t) UINT8_C(15), (std::uint8_t) UINT8_C( 7), (std::uint8_t) UINT8_C(89), (std::uint8_t) UINT8_C(33),
  (std::uint8_t) UINT8_C(77), (std::uint8_t) UINT8_C(28), (std::uint8_t) UINT8_C(65), (std::uint8_t) UINT8_C(80),
  (std::uint8_t) UINT8_C(35), (std::uint8_t) UINT8_C(71), (std::uint8_t) UINT8_C(27), (std::uint8_t) UINT8_C(90),
  (std::uint8_t) UINT8_C(91), (std::uint8_t) UINT8_C(37), (std::uint8_t) UINT8_C(67), (std::uint8_t) UINT8_C(42),
  (std::uint8_t) UINT8_C( 8), (std::uint8_t) UINT8_C( 5), (std::uint8_t) UINT8_C(65), (std::uint8_t) UINT8_C(54),
  (std::uint8_t) UINT8_C(93), (std::uint8_t) UINT8_C(62), (std::uint8_t) UINT8_C(46), (std::uint8_t) UINT8_C(46)
};

constexpr std::array<std::uint8_t, 32U> const_pi_control_tail_08_1000001
{
  // tail 1 + 10^6 std::uint8_t
  (std::uint8_t) UINT8_C(20), (std::uint8_t) UINT8_C(87), (std::uint8_t) UINT8_C(54), (std::uint8_t) UINT8_C(24),
  (std::uint8_t) UINT8_C(50), (std::uint8_t) UINT8_C(59), (std::uint8_t) UINT8_C(89), (std::uint8_t) UINT8_C(56),
  (std::uint8_t) UINT8_C(78), (std::uint8_t) UINT8_C(79), (std::uint8_t) UINT8_C(61), (std::uint8_t) UINT8_C(30),
  (std::uint8_t) UINT8_C(33), (std::uint8_t) UINT8_C(11), (std::uint8_t) UINT8_C(64), (std::uint8_t) UINT8_C(62),
  (std::uint8_t) UINT8_C(83), (std::uint8_t) UINT8_C(99), (std::uint8_t) UINT8_C(63), (std::uint8_t) UINT8_C(46),
  (std::uint8_t) UINT8_C(46), (std::uint8_t) UINT8_C( 4), (std::uint8_t) UINT8_C(22), (std::uint8_t) UINT8_C( 9),
  (std::uint8_t) UINT8_C( 1), (std::uint8_t) UINT8_C( 6), (std::uint8_t) UINT8_C(10), (std::uint8_t) UINT8_C(57),
  (std::uint8_t) UINT8_C(79), (std::uint8_t) UINT8_C(45), (std::uint8_t) UINT8_C(81), (std::uint8_t) UINT8_C(51)
};

} } //   namespace math::constants

void sys_start_callback_for_pi_digits10(const std::uint32_t d10);

static std::uint32_t sys_start_pi_count;

void sys_start_callback_for_pi_digits10(const std::uint32_t d10)
{
  char pstr[10U] = { 0 };

  const char* pend = util::baselexical_cast(d10, pstr);

  mcal::lcd::lcd0().write(pstr, (std::uint_fast8_t) (pend - pstr), 0U);

  std::fill(pstr, pstr + sizeof(pstr), (char) 0);

  pend = util::baselexical_cast(sys_start_pi_count, pstr);

  mcal::lcd::lcd0().write(pstr, (std::uint_fast8_t) (pend - pstr), 1U);
}

bool app::benchmark::run_pi_agm()
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
      sys_start_callback_for_pi_digits10
    );

  const bool head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                     my_pi.crepresentation().cbegin() + math::constants::const_pi_control_head_32.size(),
                                     math::constants::const_pi_control_head_32.begin());

  constexpr std::int32_t local_elem_digits10 =
    math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;

  using const_iterator_type = typename local_wide_decimal_type::representation_type::const_iterator;

  const_iterator_type fi(my_pi.crepresentation().cbegin() + (std::uint32_t) (  (std::uint32_t) (1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10))
                                                                             - (std::uint32_t) math::constants::const_pi_control_tail_32_1000001.size()));

  const bool tail_is_ok = std::equal(fi,
                                     fi + math::constants::const_pi_control_tail_32_1000001.size(),
                                          math::constants::const_pi_control_tail_32_1000001.begin());

  const bool result_is_ok = (head_is_ok && tail_is_ok);

  ++sys_start_pi_count;

  return result_is_ok;
}

#if defined(APP_BENCHMARK_STANDALONE_MAIN)
int main()
{
  // g++ -Wall -O3 -march=native -I./ref_app/src/mcal/host -I./ref_app/src -DAPP_BENCHMARK_TYPE=APP_BENCHMARK_TYPE_PI_AGM -DAPP_BENCHMARK_STANDALONE_MAIN ./ref_app/src/app/benchmark/app_benchmark_pi_agm.cpp -o ./ref_app/bin/app_benchmark_pi_agm.exe

  bool result_is_ok = true;

  for(unsigned i = 0U; i < 64U; ++i)
  {
    result_is_ok &= app::benchmark::run_pi_agm();
  }

  return result_is_ok ? 0 : -1;
}
#endif

