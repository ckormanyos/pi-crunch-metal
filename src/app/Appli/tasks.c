

/*******************************************************************************************************************
** Includes
*******************************************************************************************************************/
#include <TCB.h>
#include <OsAPIs.h>

//===============================================================================================================================
// OS TASK : T1
//===============================================================================================================================
TASK(T1)
{
  extern void pi_led_toggle(void);
  extern void pi_lcd_progress(void);

  pi_led_toggle();

  pi_lcd_progress();

  // Get some prime cycle times with:
  //   Table[Prime[n], {n, 60, 180, 1}]

  (void) OS_SetRelAlarm(ALARM_LED_BLINK,    0, 1009);
  (void) OS_SetRelAlarm(ALARM_LCD_PROGRESS, 0,  463);

  const OsEventMaskType OsWaitEventMask = (OsEventMaskType) (EVT_LED_BLINK | EVT_LCD_PROGRESS);

  for(;;)
  {
    if(E_OK == OS_WaitEvent(OsWaitEventMask))
    {
      OsEventMaskType Events = (OsEventMaskType) 0U;

      (void) OS_GetEvent((OsTaskType) T1, &Events);

      if((Events & EVT_LED_BLINK) == EVT_LED_BLINK)
      {
        OS_ClearEvent(EVT_LED_BLINK);

        pi_led_toggle();
      }

      if((Events & EVT_LCD_PROGRESS) == EVT_LCD_PROGRESS)
      {
        OS_ClearEvent(EVT_LCD_PROGRESS);

        pi_lcd_progress();
      }
    }
    else
    {
      /* In case of error we switch off the task */
      OS_TerminateTask();
    }
  }
}

//===============================================================================================================================
// OS TASK : Idle
//===============================================================================================================================
TASK(Idle)
{
  for(;;)
  {
    extern int pi_main(void);

    OS_Schedule();

    const int pi_result = pi_main();

    if(pi_result != 0)
    {
      /* In case of error we switch off the task */
      OS_TerminateTask();
    }
  }
}
