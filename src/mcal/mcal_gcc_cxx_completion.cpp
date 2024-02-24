///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2024.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <chrono>
#include <cstddef>

#include <mcal_cpu.h>

extern "C"
{
  // Patched labels.
  void* __dso_handle;
}

namespace std
{
  void __throw_bad_function_call();

  void __throw_bad_function_call() { while(1); }
}

void  operator delete(void*)        noexcept;
#if (defined(__GNUC__) && (__GNUC__ > 12))
inline void operator delete(void*, void*) noexcept;
#else
       void operator delete(void*, void*) noexcept;
#endif
#if(__cplusplus >= 201400L)
void operator delete(void*, std::size_t) noexcept;
#endif

void operator delete(void*)              noexcept { }
#if (defined(__GNUC__) && (__GNUC__ >= 12))
#else
void operator delete(void*, void*)       noexcept { }
#endif
#if(__cplusplus >= 201400L)
void operator delete(void*, std::size_t) noexcept { }
#endif

extern "C"
{
  // Declarations of patched functions.

  // Provide stubbed copies of certain functions declared in <stdlib.h> and <cstdlib>.
  // Also provide stubbed copies of certain empirically found library functions
  // and objects.

  int         atexit              (void (*)(void));
  void        __cxa_pure_virtual  (void);
  char*       __cxa_demangle      (const char*, char*, size_t*, int*);

  // Implementations of patched functions.

  int         atexit              (void (*)(void))                    { return 0; }
  void        __cxa_pure_virtual  (void)                              { }
  char*       __cxa_demangle      (const char*, char*, size_t*, int*) { return nullptr; }

  #if defined(environ)
  #undef environ
  #endif

  // Provide some patched data values.
  const char*  const __env[1U]       = { nullptr };
  const char** const environ         = { nullptr };

  extern int* __errno(void);
  int* __errno(void) { return nullptr; }
  std::uint8_t __fdlib_version;
}
