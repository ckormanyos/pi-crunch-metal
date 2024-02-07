///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2021 - 2024.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// cd /mnt/c/Users/User/Documents/Ks/uC_Software/Boards/OSEK_Raspberry_Pi_Zero

/*******************************************************************************************************************
** Includes
*******************************************************************************************************************/
#include <OsAPIs.h>

/*******************************************************************************************************************
** Globals 
*******************************************************************************************************************/

extern void mcal_init(void);

int main(void)
{
  mcal_init();

  OS_StartOS(APP_MODE_DEFAULT);

  return(0);
}

