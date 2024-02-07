// *****************************************************************************
// Filename    : HwPltfm.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Hardware Platform definition for ARM1176JZF-S(ARMv6)
// 
// *****************************************************************************

#ifndef HARDWARE_PLATFORM_2017_12_27_H
#define HARDWARE_PLATFORM_2017_12_27_H

#define OS_DISPATCH()            __asm("SWI #0")

#define ENABLE_INTERRUPTS()     __asm volatile("CPSIE i")
#define DISABLE_INTERRUPTS()    __asm volatile("CPSID i")

#define EXCEPTION_CODE_MASK   (uint32)0x1F
#define CPU_MODE              (uint32)0x1F

#endif /* HARDWARE_PLATFORM_2017_12_27_H */
