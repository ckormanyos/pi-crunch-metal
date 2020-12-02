
#include <mcal_cpu.h>
#include <mcal_gpt.h>
#include <mcal_irq.h>
#include <mcal_lcd.h>
#include <mcal_led.h>

#define WIDE_DECIMAL_DISABLE_IOSTREAM
#define WIDE_DECIMAL_DISABLE_DYNAMIC_MEMORY_ALLOCATION
#define WIDE_DECIMAL_DISABLE_CONVERSION_TO_BUILTINS
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_BUILTIN_FLOAT
#define WIDE_DECIMAL_DISABLE_CONSTRUCT_FROM_STRING

#include <math/constants/pi_control_head_and_tail.h>
#include <math/wide_decimal/decwide_t.h>
#include <util/memory/util_n_slot_array_allocator.h>
#include <util/utility/util_baselexical_cast.h>

void sys_start_callback_for_pi_digits10(const std::uint32_t d10);

namespace
{
  bool example002_pi();

  bool example002_pi()
  {
    using local_limb_type = std::uint32_t;

    constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(1000001);

    constexpr std::int32_t local_elem_number =
      math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_number;

    constexpr std::int32_t local_elem_digits10 =
      math::wide_decimal::detail::decwide_t_helper<wide_decimal_digits10, local_limb_type>::elem_digits10;

    using local_allocator_type = util::n_slot_array_allocator<void, local_elem_number, 16U>;

    math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, local_allocator_type, double> my_pi =
      math::wide_decimal::pi<wide_decimal_digits10, local_limb_type, local_allocator_type, double>(sys_start_callback_for_pi_digits10);

    const bool head_is_ok = std::equal(my_pi.crepresentation().cbegin(),
                                       my_pi.crepresentation().cbegin() + const_pi_control_head<local_limb_type>().size(),
                                       const_pi_control_head<local_limb_type>().begin());

    const bool tail_is_ok = std::equal(my_pi.crepresentation().cbegin() + ((std::uint32_t) (1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10)) - const_pi_control_tail<wide_decimal_digits10, local_limb_type>().size()),
                                       my_pi.crepresentation().cbegin() +  (std::uint32_t) (1UL + ((wide_decimal_digits10 - 1UL) / local_elem_digits10)),
                                       const_pi_control_tail<wide_decimal_digits10, local_limb_type>().begin());

    const bool result_is_ok = (head_is_ok && tail_is_ok);

    return result_is_ok;
  }

  std::uint32_t sys_start_pi_count;
  bool          sys_start_pi_result = true;
}

void sys_start_callback_for_pi_digits10(const std::uint32_t d10)
{
  char pstr[10U] = { 0 };

  const char* pend = util::baselexical_cast(d10, pstr);

  mcal::lcd::lcd0().write_n(pstr, (std::uint_fast8_t) (pend - pstr), 0U, true);

  std::fill(pstr, pstr + sizeof(pstr), (char) 0);

  pend = util::baselexical_cast(sys_start_pi_count, pstr);

  mcal::lcd::lcd0().write_n(pstr, (std::uint_fast8_t) (pend - pstr), 1U, true);
}

int main() __attribute__((noreturn));

int main()
{
  mcal::irq::init(nullptr);
  mcal::gpt::init(nullptr);

  (void) mcal::lcd::lcd0().init();

  mcal::cpu::init();

  while(sys_start_pi_result)
  {
    mcal::led::led0().toggle();
    
    sys_start_pi_result &= example002_pi();

    if(sys_start_pi_result)
    {
      ++sys_start_pi_count;
    }
  }

  for(;;)
  {
    mcal::led::led0().toggle();
  }
}
