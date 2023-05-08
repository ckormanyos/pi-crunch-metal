#ifndef _GPT_RPI_ZERO_BCM2835_H_
#define _GPT_RPI_ZERO_BCM2835_H_

#include"Platform_Types.h"

#define GPT_ARM_TIMER_PERIOD_USEC(x)     (uint32(x)
#define GPT_ARM_TIMER_PERIOD_MSEC(x)     (uint32)(x * 1000)
#define GPT_ARM_TIMER_PERIOD_SEC(x)      (uint32)(x * 1000 * 1000)

/***************************************************************************************************/
/* ARM Timer peripheral (Source Clock: 250MHZ) */
/***************************************************************************************************/

#define GPT_ARM_TIMER_BASE              (0x2000B000UL)
#define GPT_ARM_TIMER_LOAD_ADR          (GPT_ARM_TIMER_BASE + 0x400UL)
#define GPT_ARM_TIMER_VALUE_ADR         (GPT_ARM_TIMER_BASE + 0x404UL)
#define GPT_ARM_TIMER_CONTROL_ADR       (GPT_ARM_TIMER_BASE + 0x408UL)
#define GPT_ARM_TIMER_IRQCA_ADR         (GPT_ARM_TIMER_BASE + 0x40CUL)
#define GPT_ARM_TIMER_RAW_IRQ_ADR       (GPT_ARM_TIMER_BASE + 0x410UL)
#define GPT_ARM_TIMER_MSK_IRQ_ADR       (GPT_ARM_TIMER_BASE + 0x414UL)
#define GPT_ARM_TIMER_RELOAD_ADR        (GPT_ARM_TIMER_BASE + 0x418UL)
#define GPT_ARM_TIMER_PRE_DIV_ADR       (GPT_ARM_TIMER_BASE + 0x41CUL)
#define GPT_ARM_TIMER_FREE_RC_ADR       (GPT_ARM_TIMER_BASE + 0x420UL)

#define GPT_ARM_TIMER_LOAD         (*(volatile uint32*)GPT_ARM_TIMER_LOAD_ADR)
#define GPT_ARM_TIMER_VALUE        (*(volatile uint32*)GPT_ARM_TIMER_VALUE_ADR)
#define GPT_ARM_TIMER_CONTROL      ( (volatile sArmTimerControlReg*)GPT_ARM_TIMER_CONTROL_ADR )
#define GPT_ARM_TIMER_IRQCA        (*(volatile uint32*)GPT_ARM_TIMER_IRQCA_ADR)
#define GPT_ARM_TIMER_RAW_IRQ      ( (volatile const sArmTimerXRIS*)GPT_ARM_TIMER_RAW_IRQ_ADR )
#define GPT_ARM_TIMER_MSK_IRQ      ( (volatile const sArmTimerXMIS*)GPT_ARM_TIMER_MSK_IRQ_ADR )
#define GPT_ARM_TIMER_RELOAD       (*(volatile uint32*)GPT_ARM_TIMER_RELOAD_ADR)
#define GPT_ARM_TIMER_PRE_DIV      ( (volatile sArmTimerPreDivider*)GPT_ARM_TIMER_PRE_DIV_ADR )
#define GPT_ARM_TIMER_FREE_RC      (*(volatile uint32*)GPT_ARM_TIMER_FREE_RC_ADR)

#define GPT_ARM_TIMER_CTL_16BIT    0U
#define GPT_ARM_TIMER_CTL_32BIT    1U

#define GPT_ARM_TIMER_CTL_DIV_1    0U
#define GPT_ARM_TIMER_CTL_DIV_16   1U
#define GPT_ARM_TIMER_CTL_DIV_256  2U
#define GPT_ARM_TIMER_CTL_DIV_UND  3U

#define GPT_ARM_TIMER_CTL_INTERRUPT_DISABLED 0U
#define GPT_ARM_TIMER_CTL_INTERRUPT_ENABLED  1U

#define GPT_ARM_TIMER_CTL_MODULE_DISABLED 0U
#define GPT_ARM_TIMER_CTL_MODULE_ENABLED  1U

#define GPT_ARM_TIMER_CTL_DEBUG_RUN   0U
#define GPT_ARM_TIMER_CTL_DEBUG_STOP  1U

#define GPT_ARM_TIMER_CTL_COUNTER_DISABLED 0U
#define GPT_ARM_TIMER_CTL_COUNTER_ENABLED  1U

/* Timer control register */
typedef union
{
  struct
  {
    uint32          :1; /* Not used, The timer is always in wrapping mode. */
    uint32 TimerSize:1; /* 0 = 16-bit counter (default), 1 = 32-bit counter.*/
    uint32  TimerPre:2; /* 0 = clock is divided by 1 (default), 1 = clock is divided by 16, 2 = clock is divided by 256, 3 = undefined */
    uint32          :1;
    uint32 IntEnable:1; /* 0 = Timer module Interrupt disabled, 1 = Timer module Interrupt enabled (default) */
    uint32          :1; /* Not used, The timer is always in free running mode. */
    uint32   TimerEn:1; /* 0 = Timer module disabled (default), 1 = Timer module enabled */
    uint32 DebugMode:1; /* 0 = Timers keeps running if ARM is in debug halted mode, 1 : Timers halted if ARM is in debug halted mode */
    uint32     FRCEn:1; /* 0 = Free running counter Disabled, 1 = Free running counter Enabled */
    uint32          :6;
    uint32      FRCP:8; /* Free Running Counter Prescaler */
    uint32          :8;
  }bit;
  uint32 reg;
}sArmTimerControlReg;


/* Timer Raw IRQ register */
typedef union
{
  struct
  {
    uint32 TimerXRIS:1; /* 0 = The interrupt pending bits is clear, 1 = The interrupt pending bit is set */
    uint32          :31;
  }bit;
  uint32 reg;
}sArmTimerXRIS;


/* Timer Masked IRQ register */

typedef union
{
  struct
  {
    uint32 TimerXMIS:1; /* 0 = Interrupt line not asserted, 1 = Interrupt line is asserted (the interrupt pending and the interrupt enable bit are set) */
    uint32          :31;
  }bit;
  uint32 reg;
}sArmTimerXMIS;

/* The timer pre-divider register */
typedef union
{
  struct
  {
    uint32 PreDiv:10;
    uint32       :22;
  }bit;
  uint32 reg;
}sArmTimerPreDivider;


/*
typedef union
{
  struct
  {
  }bit;
  uint32 reg;
};
*/

void gpt_InitArmTimer(uint32 timeout);
void gpt_StartArmTimer(void);
void gpt_StopArmTimer(void);

#endif
