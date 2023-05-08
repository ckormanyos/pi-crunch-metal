// *****************************************************************************************************************
// Filename    : OsCfg.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : OSEK based application config
// 
// *****************************************************************************************************************

#include"OsGenMac.h"

//=============================================================================
//  OS Configuration
//=============================================================================

OS_CONFIG_BEGIN

  OS_TASK_BEGIN
    OS_TASK_DEF(Idle ,1 ,2048 ,1 ,OS_AUTOSTART    ,EXTENDED ,FULL_PREEMPT)
    OS_TASK_DEF(T1   ,4 ,2048 ,1 ,OS_AUTOSTART    ,EXTENDED ,FULL_PREEMPT)
  OS_TASK_END

  OS_EVENT_BEGIN
   OS_EVENT_DEF(EVT_LED_BLINK               , (1UL<<0))
   OS_EVENT_DEF(EVT_LCD_PROGRESS            , (1UL<<1))
  OS_EVENT_END

  OS_ALARM_BEGIN
    OS_ALARM_DEF(ALARM_LED_BLINK        ,ALARM_SET_EVENT     ,EVT_LED_BLINK             ,T1   ,0)
    OS_ALARM_DEF(ALARM_LCD_PROGRESS     ,ALARM_SET_EVENT     ,EVT_LCD_PROGRESS          ,T1   ,0)
  OS_ALARM_END

  OS_RESOURCE_BEGIN
    OS_RESOURCE_DEF(RES_SCHEDULER ,RES_SCHEDULER_PRIO , {1,1})
  OS_RESOURCE_END

  OS_INTERRUPT_BEGIN
    OS_INTERRUPT_CAT2_DEF(15,SysTickTimer)
  OS_INTERRUPT_END

OS_CONFIG_END
