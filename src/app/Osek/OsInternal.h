// *****************************************************************************
// Filename    : OsInternal.h
// 
// OS          : OSEK 2.2.3
// 
// CC          : ECC1/BCC1
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : OS internal services
// 
// *****************************************************************************

#ifndef OS_INTERNAL_2017_12_27_H
#define OS_INTERNAL_2017_12_27_H

#include <Platform_Types.h>

boolean OsIsInterruptContext(void);
boolean OsIsInterruptEILevelContext(void);
boolean OsIsCat2IntContext(void);

#endif /* OS_INTERNAL_2017_12_27_H */
