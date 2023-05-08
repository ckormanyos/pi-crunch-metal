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

#ifndef __OS_INTERNAL_H__
#define __OS_INTERNAL_H__

#include"Platform_Types.h"

boolean OsIsInterruptContext(void);
boolean OsIsInterruptEILevelContext(void);
boolean OsIsCat2IntContext(void);

#endif
