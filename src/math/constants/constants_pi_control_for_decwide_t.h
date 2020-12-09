#ifndef CONSTANTS_PI_CONTROL_FOR_DECWIDE_T_2020_11_21_H_
  #define CONSTANTS_PI_CONTROL_FOR_DECWIDE_T_2020_11_21_H_

  #include <cstdint>
  #include <initializer_list>

  namespace math { namespace constants {

  template<typename UnsignedIntegralType>
  constexpr std::initializer_list<UnsignedIntegralType> const_pi_control_head();

  template<const std::uint32_t Digits10,
           typename UnsignedIntegralType>
  constexpr std::initializer_list<UnsignedIntegralType> const_pi_control_tail();

  template<>
  constexpr std::initializer_list<std::uint32_t> const_pi_control_head<std::uint32_t>()
  {
    // head std::uint32_t
    return
    {
      (std::uint32_t) UINT32_C(3),
      (std::uint32_t) UINT32_C(14159265),
      (std::uint32_t) UINT32_C(35897932),
      (std::uint32_t) UINT32_C(38462643),
      (std::uint32_t) UINT32_C(38327950),
      (std::uint32_t) UINT32_C(28841971),
      (std::uint32_t) UINT32_C(69399375),
      (std::uint32_t) UINT32_C(10582097)
    };
  }

  template<>
  constexpr std::initializer_list<std::uint16_t> const_pi_control_head<std::uint16_t>()
  {
    // head std::uint16_t
    return
    {
      (std::uint16_t) UINT16_C(3),
      (std::uint16_t) UINT16_C(1415), (std::uint16_t) UINT16_C(9265),
      (std::uint16_t) UINT16_C(3589), (std::uint16_t) UINT16_C(7932),
      (std::uint16_t) UINT16_C(3846), (std::uint16_t) UINT16_C(2643),
      (std::uint16_t) UINT16_C(3832), (std::uint16_t) UINT16_C(7950),
      (std::uint16_t) UINT16_C(2884), (std::uint16_t) UINT16_C(1971),
      (std::uint16_t) UINT16_C(6939), (std::uint16_t) UINT16_C(9375),
      (std::uint16_t) UINT16_C(1058), (std::uint16_t) UINT16_C(2097)
    };
  }

  template<>
  constexpr std::initializer_list<std::uint32_t> const_pi_control_tail<10001UL, std::uint32_t>()
  {
    // tail 1 + 10^4 std::uint32_t
    return
    {
      (std::uint32_t) UINT32_C(29552498),
      (std::uint32_t) UINT32_C(87275846),
      (std::uint32_t) UINT32_C(10126483),
      (std::uint32_t) UINT32_C(69998922),
      (std::uint32_t) UINT32_C(56959688),
      (std::uint32_t) UINT32_C(15920560),
      (std::uint32_t) UINT32_C( 1016552),
      (std::uint32_t) UINT32_C(56375678)
    };
  }

  template<>
  constexpr std::initializer_list<std::uint32_t> const_pi_control_tail<100001UL, std::uint32_t>()
  {
    // tail 1 + 10^5 std::uint32_t
    return
    {
      (std::uint32_t) UINT32_C(38043299),
      (std::uint32_t) UINT32_C(70695770),
      (std::uint32_t) UINT32_C(15078933),
      (std::uint32_t) UINT32_C(77286580),
      (std::uint32_t) UINT32_C(35712790),
      (std::uint32_t) UINT32_C(91376742),
      (std::uint32_t) UINT32_C( 8056554),
      (std::uint32_t) UINT32_C(93624646)
    };
  }

  template<>
  constexpr std::initializer_list<std::uint32_t> const_pi_control_tail<1000001UL, std::uint32_t>()
  {
    // tail 1 + 10^6 std::uint32_t
    return
    {
      (std::uint32_t) UINT32_C(20875424),
      (std::uint32_t) UINT32_C(50598956),
      (std::uint32_t) UINT32_C(78796130),
      (std::uint32_t) UINT32_C(33116462),
      (std::uint32_t) UINT32_C(83996346),
      (std::uint32_t) UINT32_C(46042209),
      (std::uint32_t) UINT32_C( 1061057),
      (std::uint32_t) UINT32_C(79458151)
    };
  }

  template<>
  constexpr std::initializer_list<std::uint16_t> const_pi_control_tail<10001UL, std::uint16_t>()
  {
    // tail 1 + 10^4 std::uint16_t
    return
    {
      (std::uint16_t) UINT16_C(2955), (std::uint16_t) UINT16_C(2498),
      (std::uint16_t) UINT16_C(8727), (std::uint16_t) UINT16_C(5846),
      (std::uint16_t) UINT16_C(1012), (std::uint16_t) UINT16_C(6483),
      (std::uint16_t) UINT16_C(6999), (std::uint16_t) UINT16_C(8922),
      (std::uint16_t) UINT16_C(5695), (std::uint16_t) UINT16_C(9688),
      (std::uint16_t) UINT16_C(1592), (std::uint16_t) UINT16_C(0560),
      (std::uint16_t) UINT16_C( 101), (std::uint16_t) UINT16_C(6552),
      (std::uint16_t) UINT16_C(5637), (std::uint16_t) UINT16_C(5678)
    };
  }

  template<>
  constexpr std::initializer_list<std::uint16_t> const_pi_control_tail<100001UL, std::uint16_t>()
  {
    // tail 1 + 10^5 std::uint16_t
    return
    {
      (std::uint16_t) UINT16_C(3804), (std::uint16_t) UINT16_C(3299),
      (std::uint16_t) UINT16_C(7069), (std::uint16_t) UINT16_C(5770),
      (std::uint16_t) UINT16_C(1507), (std::uint16_t) UINT16_C(8933),
      (std::uint16_t) UINT16_C(7728), (std::uint16_t) UINT16_C(6580),
      (std::uint16_t) UINT16_C(3571), (std::uint16_t) UINT16_C(2790),
      (std::uint16_t) UINT16_C(9137), (std::uint16_t) UINT16_C(6742),
      (std::uint16_t) UINT16_C( 805), (std::uint16_t) UINT16_C(6554),
      (std::uint16_t) UINT16_C(9362), (std::uint16_t) UINT16_C(4646)
    };
  }

  template<>
  constexpr std::initializer_list<std::uint16_t> const_pi_control_tail<1000001UL, std::uint16_t>()
  {
    // tail 1 + 10^6 std::uint16_t
    return
    {
      (std::uint16_t) UINT16_C(2087), (std::uint16_t) UINT16_C(5424),
      (std::uint16_t) UINT16_C(5059), (std::uint16_t) UINT16_C(8956),
      (std::uint16_t) UINT16_C(7879), (std::uint16_t) UINT16_C(6130),
      (std::uint16_t) UINT16_C(3311), (std::uint16_t) UINT16_C(6462),
      (std::uint16_t) UINT16_C(8399), (std::uint16_t) UINT16_C(6346),
      (std::uint16_t) UINT16_C(4604), (std::uint16_t) UINT16_C(2209),
      (std::uint16_t) UINT16_C( 106), (std::uint16_t) UINT16_C(1057),
      (std::uint16_t) UINT16_C(7945), (std::uint16_t) UINT16_C(8151)
    };
  }

  } } //   namespace math::constants

#endif // CONSTANTS_PI_CONTROL_FOR_DECWIDE_T_2020_11_21_H_