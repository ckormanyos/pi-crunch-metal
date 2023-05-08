#ifndef _SYS_TIMER_RPI_ZERO_BCM2835_H_
#define _SYS_TIMER_RPI_ZERO_BCM2835_H_

#include"Platform_Types.h"

/***************************************************************************************************/
/* System Timer peripheral (Source Clock: 1MHZ) */
/***************************************************************************************************/

#define SYSTEM_TIMER_BASE             (0x20003000UL)
#define SYSTEM_TIMER_CS_ADR           (SYSTEM_TIMER_BASE + 0x00UL)
#define SYSTEM_TIMER_CLO_ADR          (SYSTEM_TIMER_BASE + 0x04UL)
#define SYSTEM_TIMER_CHI_ADR          (SYSTEM_TIMER_BASE + 0x08UL)
#define SYSTEM_TIMER_C0_ADR           (SYSTEM_TIMER_BASE + 0x0CUL)
#define SYSTEM_TIMER_C1_ADR           (SYSTEM_TIMER_BASE + 0x10UL)
#define SYSTEM_TIMER_C2_ADR           (SYSTEM_TIMER_BASE + 0x14UL)
#define SYSTEM_TIMER_C3_ADR           (SYSTEM_TIMER_BASE + 0x18UL)


#define SYSTEM_TIMER_CS    ((volatile sCsReg*)SYSTEM_TIMER_CS_ADR )
#define SYSTEM_TIMER_CLO   (*(volatile uint32*)SYSTEM_TIMER_CLO_ADR)
#define SYSTEM_TIMER_CHI   (*(volatile uint32*)SYSTEM_TIMER_CHI_ADR)
#define SYSTEM_TIMER_C0    (*(volatile uint32*)SYSTEM_TIMER_C0_ADR ) /* Used by the GPU in RPI */
#define SYSTEM_TIMER_C1    (*(volatile uint32*)SYSTEM_TIMER_C1_ADR )
#define SYSTEM_TIMER_C2    (*(volatile uint32*)SYSTEM_TIMER_C2_ADR ) /* Used by the GPU in RPI */
#define SYSTEM_TIMER_C3    (*(volatile uint32*)SYSTEM_TIMER_C3_ADR )


#define SYSTEM_TIMER_NO_TIMER_X_MATCH_SINCE_LAST_CLEARED   0U
#define SYSTEM_TIMER_TIMER_X_MATCH_DETECTED                1U


typedef union
{
  struct
  {
    uint32 M0:1;
    uint32 M1:1;
    uint32 M2:1;
    uint32 M3:1;
    uint32   :28;
  }bit;
  uint32 reg;
}sCsReg;

#define SYSTEM_TIMER_PERIODIC_USEC(x)     x
#define SYSTEM_TIMER_PERIODIC_MSEC(x)     x * 1000
#define SYSTEM_TIMER_PERIODIC_SEC(x)      x * 1000 * 1000




void SysTickTimer_Init(void);
void SysTickTimer_Start(void);
boolean SysTickTimer_FIQCheckAndUpdate(void);
void SysTickTimer_Delay(uint32 delay);


#endif
