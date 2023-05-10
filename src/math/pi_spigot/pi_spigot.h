///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022.                               //
//  Copyright Christopher Kormanyos 2019 - 2023.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#ifndef PI_SPIGOT_2022_06_08_H
  #define PI_SPIGOT_2022_06_08_H

  #include <algorithm>
  #include <array>
  #include <functional>

  #include <math/checksums/hash/hash_base.h>

  #if (__cplusplus >= 201703L)
  namespace math::constants {
  #else
  namespace math { namespace constants {
  #endif

  template<const std::uint32_t ResultDigit,
           const std::uint32_t LoopDigit,
           typename UnsignedSmallType = std::uint32_t,
           typename UnsignedLargeType = std::uint64_t>
  class pi_spigot
  {
  private:
    using unsigned_small_type = UnsignedSmallType;
    using unsigned_large_type = UnsignedLargeType;

    static constexpr auto result_digit() -> std::uint32_t { return ResultDigit; }
    static constexpr auto loop_digit  () -> std::uint32_t { return LoopDigit; }

    static_assert(loop_digit() <= static_cast<std::uint32_t>(std::numeric_limits<unsigned_small_type>::digits10),
                  "Error: loop_digit exceeds the number of base-10 digits in its constituent unsigned_small_type");

    static_assert(result_digit() <= static_cast<std::uint32_t>(UINT32_C(100001)),
                  "Error: result_digit exceeds its limit of 100,001");

    static_assert(std::numeric_limits<unsigned_small_type>::digits * 2 == std::numeric_limits<unsigned_large_type>::digits,
                  "Error: unsigned_large_type must be exactly twice as wide as unsigned_small_type");

    static_assert((!std::numeric_limits<unsigned_small_type>::is_signed),
                  "Error: unsigned_small_type must be unsigned");

    static_assert((!std::numeric_limits<unsigned_large_type>::is_signed),
                  "Error: unsigned_large_type must be unsigned");

    static constexpr auto input_scale(std::uint32_t x) -> std::uint32_t
    {
      return
        static_cast<std::uint32_t>
        (
          static_cast<std::uint32_t>
          (
              x
            * static_cast<std::uint32_t>
              (
                  static_cast<std::uint32_t>
                  (
                      static_cast<std::uint32_t>(static_cast<std::uint32_t>(UINT8_C(10)) * loop_digit())
                    / static_cast<std::uint32_t>(UINT8_C(3))
                  )
                + static_cast<std::uint32_t>(UINT8_C(1))
              )
          )
          / loop_digit()
        );
    }

  public:
    static constexpr auto input_static_size = input_scale(result_digit());

    static constexpr auto get_input_static_size() -> std::uint32_t { return input_static_size; }

    using input_container_type = std::array<std::uint32_t, static_cast<std::size_t>(input_static_size)>;

    constexpr pi_spigot() = default; // LCOV_EXCL_LINE

    constexpr pi_spigot(const pi_spigot&) = delete;

    constexpr pi_spigot(pi_spigot&&) = delete;

    ~pi_spigot() = default; // LCOV_EXCL_LINE

    constexpr auto operator=(const pi_spigot&) -> pi_spigot& = delete;

    constexpr auto operator=(pi_spigot&&) -> pi_spigot& = delete;

    [[nodiscard]] constexpr auto get_operation_count() const -> std::uintmax_t
    {
      return my_operation_count;
    }

    auto calculate(std::function<void(const std::uint32_t)> pfn_callback_to_report_digits10 = nullptr,
                   ::math::checksums::hash::hash_stream_base* p_hash = nullptr) -> void
    {
      // Use pi_spigot::calculate() to calculate result_digit
      // decimal digits of pi.

      // The caller is responsible for providing the output memory
      // for the result of pi. The input memory used for internal
      // calculation details while performing the calculation is
      // provided by the pi_spigot class itself.

      if(p_hash != nullptr)
      {
        p_hash->initialize();
      }

      // Invalidate the input container values at a few indices.
      const auto invalidate_size =
        (std::min)(static_cast<typename input_container_type::size_type>(UINT8_C(4)),
                   static_cast<typename input_container_type::size_type>(std::tuple_size<input_container_type>::value));

      std::fill(my_pi_in.begin(),
                my_pi_in.begin() + invalidate_size,
                (std::numeric_limits<typename input_container_type::value_type>::max)());

      auto val_c = static_cast<unsigned_small_type>(static_cast<unsigned>(UINT8_C(0)));

      my_output_count    = static_cast<std::uint32_t>(UINT8_C(0));
      my_operation_count = static_cast<std::uintmax_t>(UINT8_C(0));

      if(pfn_callback_to_report_digits10 != nullptr)
      {
        pfn_callback_to_report_digits10(my_output_count);
      }

      const auto p10 = static_cast<unsigned_large_type>(pow10(loop_digit()));

      // Operation count Mathematica(R), example for loop_digit=9.
      // Sum[Floor[((d - j) (Floor[((10 9)/3)] + 1))/9], {j, 0, Floor[d/9] 9, 9}]

      for(auto j = static_cast<std::uint32_t>(UINT8_C(0));
               j < result_digit();
               j = static_cast<std::uint32_t>(j + loop_digit()))
      {
        auto val_d = static_cast<unsigned_large_type>(UINT8_C(0));

        const auto ilim = input_scale(result_digit() - j);

        for(auto   i = static_cast<std::uint32_t>(UINT8_C(0));
                   i < ilim;
                 ++i)
        {
          const auto my_index =
            static_cast<std::uint32_t>
            (
                static_cast<std::uint32_t>(ilim - static_cast<std::uint32_t>(UINT8_C(1)))
              - i
            );

          const auto di =
            ((j == static_cast<std::uint32_t>(UINT8_C(0)))
              ? static_cast<unsigned_large_type>(d_init())
              : static_cast<unsigned_large_type>(my_pi_in[my_index]));

          val_d =
            static_cast<unsigned_large_type>
            (
                val_d
              + static_cast<unsigned_large_type>(di * p10)
            );

          const auto val_b =
            static_cast<std::uint32_t>
            (
                static_cast<std::uint32_t>
                (
                  my_index * static_cast<std::uint32_t>(UINT8_C(2))
                )
              + static_cast<std::uint32_t>(UINT8_C(1))
            );

          my_pi_in[my_index] = static_cast<std::uint32_t>(val_d % val_b);

          val_d /= val_b;

          if(my_index > static_cast<std::uint32_t>(UINT8_C(1)))
          {
            val_d *= my_index;
          }

          ++my_operation_count;
        }

        // Parse the next digits of pi, where the group has loop_digit digits.
        // If loop_digit is 4, for instance, then successive groups
        // of digits have a form such as: 3141, 5926, ..., etc.

        const auto p10_loop = static_cast<unsigned_large_type>(pow10(loop_digit()));

        const auto next_digits =
          static_cast<unsigned_small_type>
          (
            val_c + static_cast<unsigned_small_type>(val_d / p10_loop)
          );

        val_c = static_cast<unsigned_small_type>(val_d % p10_loop);

        const auto n =
          (std::min)
          (
            loop_digit(),
            static_cast<std::uint32_t>(result_digit() - j)
          );

        auto scale10 = pow10(loop_digit() - static_cast<std::uint32_t>(UINT8_C(1)));

        auto output_chars_as_bytes_hash_array =
          std::array<std::uint8_t, static_cast<std::size_t>(loop_digit())> { };

        for(auto i = static_cast<std::size_t>(UINT8_C(0)); i < static_cast<std::size_t>(n); ++i)
        {
          const auto output_value =
            static_cast<std::uint_fast8_t>
            (
              static_cast<unsigned_small_type>
              (
                  static_cast<unsigned_small_type>(next_digits / scale10)
                % static_cast<unsigned>(UINT8_C(10))
              )
            );

          if(p_hash != nullptr)
          {
            output_chars_as_bytes_hash_array[i] =
              static_cast<std::uint8_t>
              (
                output_value + static_cast<std::uint8_t>(UINT8_C(0x30))
              );
          }

          scale10 = static_cast<unsigned_small_type>(scale10 / static_cast<unsigned>(UINT8_C(10)));
        }

        if(p_hash != nullptr)
        {
          p_hash->process(output_chars_as_bytes_hash_array.data(), static_cast<std::size_t>(n));
        }

        my_output_count += n;

        if(pfn_callback_to_report_digits10 != nullptr)
        {
          pfn_callback_to_report_digits10(my_output_count);
        }
      }

      if(p_hash != nullptr)
      {
        p_hash->finalize();
      }
    }

    auto get_output_count() const noexcept -> std::uint32_t { return my_output_count; }

  private:
    static input_container_type my_pi_in;

    std::uintmax_t my_operation_count { };
    std::uint32_t  my_output_count    { };

    static constexpr auto pow10(std::uint32_t n) -> unsigned_small_type
    {
      return
      (
        (n == static_cast<std::uint32_t>(UINT8_C(0)))
          ? static_cast<unsigned_small_type>(UINT8_C(1))
          : static_cast<unsigned_small_type>
            (
                pow10(n - static_cast<std::uint32_t>(UINT8_C(1)))
              * static_cast<unsigned>(UINT8_C(10))
            )
      );
    }

    static constexpr auto d_init() -> unsigned_small_type
    {
      return
        static_cast<unsigned_small_type>
        (
          pow10(loop_digit()) / static_cast<unsigned>(UINT8_C(5))
        );
    }
  };

  template<const std::uint32_t ResultDigit,
           const std::uint32_t LoopDigit,
           typename UnsignedSmallType,
           typename UnsignedLargeType>
  typename pi_spigot<ResultDigit, LoopDigit, UnsignedSmallType, UnsignedLargeType>::input_container_type pi_spigot<ResultDigit, LoopDigit, UnsignedSmallType, UnsignedLargeType>::my_pi_in;

  #if (__cplusplus >= 201703L)
  } // namespace math::constants
  #else
  } // namespace constants
  } // namespace math
  #endif

#endif // PI_SPIGOT_2022_06_08_H
