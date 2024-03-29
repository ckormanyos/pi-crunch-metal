///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2024.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <algorithm>
#include <cstddef>
#include <cstdint>

extern "C"
{
  extern std::uintptr_t _rom_data_begin; // Start address for the initialization values of the rom-to-ram section.
  extern std::uintptr_t _data_begin;     // Start address for the .data section.
  extern std::uintptr_t _data_end;       // End address for the .data section.
  extern std::uintptr_t _bss_begin;      // Start address for the .bss section.
  extern std::uintptr_t _bss_end;        // End address for the .bss section.
}

namespace crt
{
  void init_ram() __attribute__((section(".startup"), used, noinline));
}

void crt::init_ram()
{
  // Note: Use a copy/fill memory-aligned type of one single byte
  // even though the sections are aligned by 4. The standard-library
  // copy/fill functions will internally use optimized word-size.

  typedef std::uint8_t memory_aligned_type;

  // Copy the data segment initializers from ROM to RAM.
  const std::size_t size_data =
    std::size_t(  static_cast<const memory_aligned_type*>(static_cast<const void*>(&_data_end))
                - static_cast<const memory_aligned_type*>(static_cast<const void*>(&_data_begin)));

  std::copy(static_cast<const memory_aligned_type*>(static_cast<const void*>(&_rom_data_begin)),
            static_cast<const memory_aligned_type*>(static_cast<const void*>(&_rom_data_begin)) + size_data,
            static_cast<      memory_aligned_type*>(static_cast<      void*>(&_data_begin)));

  // Clear the bss segment.
  std::fill(static_cast<memory_aligned_type*>(static_cast<void*>(&_bss_begin)),
            static_cast<memory_aligned_type*>(static_cast<void*>(&_bss_end)),
            static_cast<memory_aligned_type>(0U));
}
