// *****************************************************************************************************************
// Filename    : OsAsm.h
// 
// Core        : ARM1176JZF-S(ARMv6)
//
// Compiler    : ARM GCC
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 15.03.2020
// 
// Description : OSEK/ECC1 Context switch and ISR category 2 wrapper for ARM1176JZF-S(ARMv6)
// 
// *****************************************************************************************************************

#ifndef OS_ASM_2017_12_27_H
#define OS_ASM_2017_12_27_H

__attribute__((naked)) void OsDispatcher(void);
__attribute__((naked)) uint32 OsGetCurrentSP(void);
__attribute__((naked)) uint32 OsGetCPSR(void);
__attribute__((naked)) void OsCat2IsrIrqWrapper(void);
__attribute__((naked)) void OsCat2IsrFiqWrapper(void);
__attribute__((naked)) void OsStartNewTask(uint32 StackFramePtr);

#endif /* OS_ASM_2017_12_27_H */
