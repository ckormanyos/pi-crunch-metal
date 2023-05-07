// ******************************************************************************************************
// Filename    : TCB.c
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
// Description : Task Control Block definition
// 
// ******************************************************************************************************

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"
#endif

#include"TCB.h"

/***************************************************/
/*            X Macro System Generation            */
/***************************************************/

#define OS_GEN_DATA_STRUCT
#include"OsCfg.h"
#undef OS_GEN_DATA_STRUCT

#define OS_GEN_TCB
#include"OsCfg.h"
#undef OS_GEN_TCB

#define OS_GEN_OCB
#include"OsCfg.h"
#undef OS_GEN_OCB

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
