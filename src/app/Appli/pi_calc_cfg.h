///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PI_CALC_CFG_2023_05_09_H
  #define PI_CALC_CFG_2023_05_09_H

  #define PI_CRUNCH_METAL_CFG_USE_PI_AGM    1
  #define PI_CRUNCH_METAL_CFG_USE_PI_SPIGOT 2

  //#define PI_CRUNCH_METAL_CFG_METHOD PI_CRUNCH_METAL_CFG_USE_PI_AGM
  //#define PI_CRUNCH_METAL_CFG_METHOD PI_CRUNCH_METAL_CFG_USE_PI_SPIGOT

  #if !defined(PI_CRUNCH_METAL_CFG_METHOD)
  #define PI_CRUNCH_METAL_CFG_METHOD PI_CRUNCH_METAL_CFG_USE_PI_AGM
  #endif

  #if defined(_MSC_VER) && !defined(PI_CRUNCH_METAL_STANDALONE_MAIN)
  #define PI_CRUNCH_METAL_STANDALONE_MAIN
  #endif

  //#define PI_CRUNCH_METAL_CFG_PI_AGM_USE_1K_DIGITS
  //#define PI_CRUNCH_METAL_CFG_PI_AGM_USE_10K_DIGITS
  //#define PI_CRUNCH_METAL_CFG_PI_AGM_USE_100K_DIGITS
  //#define PI_CRUNCH_METAL_CFG_PI_AGM_USE_1M_DIGITS

  //#define PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_1K_DIGITS
  //#define PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_10K_DIGITS
  //#define PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_100K_DIGITS

  #if (defined(_MSC_VER) && defined(_DEBUG) && !defined(PI_CRUNCH_METAL_CFG_PI_AGM_USE_100K_DIGITS))
  #define PI_CRUNCH_METAL_CFG_PI_AGM_USE_100K_DIGITS
  #endif

  #if (defined(_MSC_VER) && defined(_DEBUG) && !defined(PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_10K_DIGITS))
  #define PI_CRUNCH_METAL_CFG_PI_SPIGOT_USE_10K_DIGITS
  #endif

#endif // PI_CALC_CFG_2023_05_09_H
