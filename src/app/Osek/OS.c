// *****************************************************************************************************************
// Filename    : OS.c
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
// Description : System services implementation
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"OsAPIs.h"
#include"TCB.h"
#include"OsAsm.h"
#include"gpt.h"
#include"mcu.h"

//------------------------------------------------------------------------------------------------------------------
// Static function prototypes
//------------------------------------------------------------------------------------------------------------------
static void OS_InitTimer(void);
static void OS_StartTimer(void);
static void OS_IdleLoop(void);
static void OsCreateNewContext(uint32* StackFramePtr, pFunc TaskFuncPtr);

//------------------------------------------------------------------------------------------------------------------
// Extern function prototypes
//------------------------------------------------------------------------------------------------------------------
extern void OS_AlarmsManagement(void);
extern void SysTickTimer_Init(void);
extern void SysTickTimer_Start(void);
extern boolean SysTickTimer_FIQCheckAndUpdate(void);

//------------------------------------------------------------------------------------------------------------------
// I/O debug
//------------------------------------------------------------------------------------------------------------------
extern void IODebug_Init(void);
extern void IODebug_Toggle_P123(void);
//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_StartOS
///
/// \descr  The user can call this system service to start the operating system in a specific mode
///
/// \param  OsAppModeType Mode
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_StartOS(OsAppModeType Mode)
{
  (void) Mode;

  if(TRUE == OsIsInterruptContext())
  {
    return;
  }

  if(NB_OF_TASKS > 0)
  {
    /* INIT TCBs */
    for(int tcbIdx = 0; tcbIdx < NB_OF_TASKS; tcbIdx++)
    {
      /* Init all stacks with the marker 0xAA */
      const uint32 stack_size = OCB_Cfg.pTcb[tcbIdx]->pstack_top - OCB_Cfg.pTcb[tcbIdx]->pstack_bot + sizeof(uint32);

      for(unsigned int offset=0;(offset < stack_size);(offset += sizeof(uint32)))
      {
        *(volatile uint32*)((OCB_Cfg.pTcb[tcbIdx]->pstack_bot) + offset) = (uint32)0xAAAAAAAA;
      }
      
      /* Set default tasks priorities */
      OCB_Cfg.pTcb[tcbIdx]->Prio = OCB_Cfg.pTcb[tcbIdx]->FixedPrio;
    }

    /* Init system tick timer */
    OS_InitTimer();

    /* Start all autostart task */
    for(int tcbIdx = 0; tcbIdx < NB_OF_TASKS; tcbIdx++)
    {
      if(OCB_Cfg.pTcb[tcbIdx]->Autostart == OS_AUTOSTART && OCB_Cfg.pTcb[tcbIdx]->TaskStatus == SUSPENDED)
      {
        /* Switch to PRE_READY state */
        OCB_Cfg.pTcb[tcbIdx]->TaskStatus = PRE_READY;

        /* Update number of activation */
        OCB_Cfg.pTcb[tcbIdx]->NbOfActiv--;
      }
    }

    /* Call startup hook function */
    #if(STARTUPHOOK)
    StartupHook();
    #endif

    /* Start system tick timer */
    OS_StartTimer();

    /* Enable the interrupts */
    ENABLE_INTERRUPTS();

    /* Start all relative autostart alarms */
    for(int alarmIdx = 0; alarmIdx < NB_OF_ALARMS; alarmIdx++)
    {
      if(OCB_Cfg.pAlarm[alarmIdx]->AutoStart == ALARM_AUTOSTART)
      {
        OS_SetRelAlarm((OsAlarmType)alarmIdx,OCB_Cfg.pAlarm[alarmIdx]->InitTicks,OCB_Cfg.pAlarm[alarmIdx]->InitCycles);
      }
    }

    /* Call Scheduler */
    (void)OS_Schedule();

    /* Set the OS running flag */
    OCB_Cfg.OsIsRunning = OS_TRUE;

    /* Call the dispatcher */
    OS_DISPATCH();
  }

  /* Infinite loop */
  OS_IdleLoop();
}


//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_Schedule
///
/// \descr  If a higher-priority task is ready, the internal resource of the task is released,
///         the current task is put into the ready state, its context is saved and the higher-priority task
///         is executed. Otherwise the calling task is continued.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_Schedule(void)
{
  if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS &&
     OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->CeilingPrio != 0 &&
     OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->Prio != OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->FixedPrio &&
     FALSE == OsIsCat2IntContext())
  {
    return(E_OS_RESOURCE);
  }
  else if(TRUE == OsIsInterruptContext() && FALSE == OsIsCat2IntContext()) /* Cat1 Interrupt */
  {
    return(E_OS_CALLEVEL);
  }
  else
  {
    sint32 HighPrio = -1;
    OCB_Cfg.HighPrioReadyTaskIdx = INVALID_TASK;

    /* Starting a critical section */
    if(OCB_Cfg.OsIsrInterruptLevel == OS_FALSE)
    {
      DISABLE_INTERRUPTS();
    }

    for(int tcbIdx = 0; tcbIdx < NB_OF_TASKS; tcbIdx++)
    {
      if(OCB_Cfg.pTcb[tcbIdx]->TaskStatus == PRE_READY ||
         OCB_Cfg.pTcb[tcbIdx]->TaskStatus == READY     ||
         OCB_Cfg.pTcb[tcbIdx]->TaskStatus == RUNNING)
      {
        if((sint32)OCB_Cfg.pTcb[tcbIdx]->Prio > (sint32)HighPrio)
        {
          HighPrio = (sint32)OCB_Cfg.pTcb[tcbIdx]->Prio;
          OCB_Cfg.HighPrioReadyTaskIdx = (uint32)tcbIdx;
        }
      }
    }

    /* Ending the critical section */
    if(OCB_Cfg.OsIsrInterruptLevel == OS_FALSE)
    {
      ENABLE_INTERRUPTS();
    }

    if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS && OCB_Cfg.HighPrioReadyTaskIdx < NB_OF_TASKS)
    {
      if(HighPrio > (sint32)(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->Prio) || 
         OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == WAITING             || 
         OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == SUSPENDED
        )
      {
        /* Call PostTaskHook */
        #if(POSTTASKHOOK)
        PostTaskHook();
        #endif

        if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == RUNNING)
        {
          /* change the state of the current task */
          OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus = READY;
        }

        if(OCB_Cfg.OsIsrInterruptLevel == OS_FALSE && OCB_Cfg.OsLockDispatcher == OS_FALSE)
        {
          /* Call the dispatcher */
          OS_DISPATCH();
        }
        else if(OCB_Cfg.OsLockDispatcher == OS_TRUE)
        {
          return(E_OK);
        }
        else
        {
          /*  OS_Schedule is called in interrupt context  */
          /*  the Dispatcher will be executed later after */
          /*  the execution of the ISR                    */
          OCB_Cfg.OsIsrCallDispatcher = OS_TRUE;
        }
      }
    }
    else
    {
      /* check if the OS is already started */
      if(OCB_Cfg.OsIsRunning)
      {
        /* No active or ready task */

        /* Call PostTaskHook */
        if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
        {
          #if(POSTTASKHOOK)
          PostTaskHook();
          #endif
        }

        if(OCB_Cfg.OsIsrInterruptLevel == OS_FALSE)
        {
          /* OS_Schedule is called outside an interrupt context */
          /* Call the dispatcher */
          OS_DISPATCH();
        }
        else
        {
          /* OS_Schedule is called in interrupt context
             the Dispatcher will be executed later after
             the execution of the ISR */
          OCB_Cfg.OsIsrCallDispatcher = OS_TRUE;
        }
      }
      else
      {
        /* Os not yet started */
      }
    }
    return(E_OK);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_InitTimer
///
/// \descr  Init the HW timer used as system tick timer
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void OS_InitTimer(void)
{
  SysTickTimer_Init();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_StartTimer
///
/// \descr  Start the HW timer used as system tick timer
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void OS_StartTimer(void)
{
  ENABLE_BASIC_IRQS = 1;
  gpt_InitArmTimer(GPT_ARM_TIMER_PERIOD_MSEC(1));
  gpt_StartArmTimer();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_Dispatcher
///
/// \descr  Context switch engine
///
/// \param  uint32 StackPtr: Current stack pointer of the active task
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
uint32 OS_Dispatcher(uint32 StackPtr);

uint32 OS_Dispatcher(uint32 StackPtr)
{
  /* Save the current stack pointer of the running task before switching the context */
  if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
  {
    OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer = StackPtr;

    /* Check the stack pointer against stack overflow */
    if(!(StackPtr <(uint32)(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pstack_top) &&
       StackPtr >= (uint32)(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pstack_bot))
      )
    {
      /* Stack overflow */
      #if(ERRORHOOK)
      ErrorHook(E_OS_STACKFAULT);
      #endif
    }
  }
  else
  {
    /* save the system stack */
    OCB_Cfg.OsCurrentSystemStackPtr = StackPtr;
  }

  /* Set the new current task */
  OCB_Cfg.CurrentTaskIdx = OCB_Cfg.HighPrioReadyTaskIdx;

  if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)  
  {
    /* Check if we need to create a new stack frame for the new task */
    if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == PRE_READY)
    {
      /* Update the current task state */
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus = RUNNING;

      /* Call PreTaskHook */
      #if(PRETASKHOOK)
      PreTaskHook();
      #endif

      /* Create Stack Frame for the 1st execution */
            uint32 NewStackFramePtr = OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pstack_top;
      const pFunc  NewThread        = OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->function;

      OsCreateNewContext(&NewStackFramePtr, NewThread);

      /* Save the new stack ptr */
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer = NewStackFramePtr;

      /* Start the new task */
      OsStartNewTask(NewStackFramePtr);

    }
    else if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == READY)
    {
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus = RUNNING;

      /* Call PreTaskHook */
      #if(PRETASKHOOK)
      PreTaskHook();
      #endif
    }
  }
  else
  {
    /* there is no ready task in the system 
       The OS will stay in endless loop until
       an event will be occurred */
    return(OCB_Cfg.OsCurrentSystemStackPtr);
  }

  return(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsCreateNewContext
///
/// \descr  This function create and fill the new task context
///
/// \param  uint32 StackFramePtr: Current stack pointer of the active task
///            pFunc TaskFuncPtr: task function pointer
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void OsCreateNewContext(uint32* StackFramePtr, pFunc TaskFuncPtr)
{
  *(volatile uint32*)(*StackFramePtr - 0x04) = (uint32)0x00;                           //r12
  *(volatile uint32*)(*StackFramePtr - 0x08) = (uint32)0x00;                           //r11
  *(volatile uint32*)(*StackFramePtr - 0x0C) = (uint32)0x00;                           //r10
  *(volatile uint32*)(*StackFramePtr - 0x10) = (uint32)0x00;                           //r9
  *(volatile uint32*)(*StackFramePtr - 0x14) = (uint32)0x00;                           //r8
  *(volatile uint32*)(*StackFramePtr - 0x18) = (uint32)0x00;                           //r7
  *(volatile uint32*)(*StackFramePtr - 0x1C) = (uint32)0x00;                           //r6
  *(volatile uint32*)(*StackFramePtr - 0x20) = (uint32)0x00;                           //r5
  *(volatile uint32*)(*StackFramePtr - 0x24) = (uint32)0x00;                           //r4
  *(volatile uint32*)(*StackFramePtr - 0x28) = (uint32)0x00;                           //r3
  *(volatile uint32*)(*StackFramePtr - 0x2C) = (uint32)0x00;                           //r2
  *(volatile uint32*)(*StackFramePtr - 0x30) = (uint32)0x00;                           //r1
  *(volatile uint32*)(*StackFramePtr - 0x34) = (uint32)0x00;                           //r0
  *(volatile uint32*)(*StackFramePtr - 0x38) = (uint32)&osErrTaskExitWithoutTerminate; //LR
  *(volatile uint32*)(*StackFramePtr - 0x3C) = (uint32)TaskFuncPtr;                    //PC
  *(volatile uint32*)(*StackFramePtr - 0x40) = (uint32)0x11F;                          //CPSR

  /* Update the stack pointer */
  *StackFramePtr -= 0x40;
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  Interrupt service routine
///
/// \descr  ISR of system tick interrupt
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
ISR(SysTickTimer)
{
  if((IRQ_BASIC_PENDING & 0x01UL) == 0x01UL)
  {
    GPT_ARM_TIMER_IRQCA = 1;
    OCB_Cfg.OsSysTickCounter++;
    OS_AlarmsManagement();
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsStoreStackPointer
///
/// \descr  Store the current stack pointer in case of category 2 interrupt
///
/// \param  uint32 StackPtrValue: Current stack pointer
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsStoreStackPointer(uint32 StackPtrValue);

void OsStoreStackPointer(uint32 StackPtrValue)
{
  OCB_Cfg.OsIsrInterruptLevel = 1;

  if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
  {
    OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer = StackPtrValue;
  }
  else
  {
    OCB_Cfg.OsCurrentSystemStackPtr = StackPtrValue;
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetSavedStackPointer
///
/// \descr  return the saved stack pointer in case of category 2 interrupt
///
/// \param  void
///
/// \return uint32 : saved stack pointer
//------------------------------------------------------------------------------------------------------------------
uint32 OsGetSavedStackPointer(void);

uint32 OsGetSavedStackPointer(void)
{
  OCB_Cfg.OsIsrInterruptLevel = 0;

  if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
  {
    return(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer);
  }
  else
  {
    return(OCB_Cfg.OsCurrentSystemStackPtr);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIsrCallDispatch
///
/// \descr  Call the dispatcher to switch the context if needed after an category 2 interrupt 
///
/// \param  uint32 StackPtr: Current stack pointer
///
/// \return uint32 : The new stack pointer after switching the context otherwise the last saved stack pointer
//------------------------------------------------------------------------------------------------------------------
uint32 OsIsrCallDispatch(uint32 StackPtr);

uint32 OsIsrCallDispatch(uint32 StackPtr)
{
  if(OCB_Cfg.OsIsrCallDispatcher == 1)
  {
    /* The internal system state is changed by the ISR cat2 (an Event is triggred)
       at this point the OS will switch to the new context (target task context) */

    /* Reset the flag */
    OCB_Cfg.OsIsrCallDispatcher = 0;

    /* Call the Dispatcher to execute the context switch process */
    return(OS_Dispatcher(StackPtr));
  }
  else
  {
    /* The system behavior is not changed by the ISR cat2 
       The cpu will restore the context saved by the interrupt*/
    if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
    {
      /* return the stack pointer of the current task */
      return(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer);
    }
    else
    {
      /* return the system stack pointer */
      return(StackPtr);
    }
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetActiveApplicationMode
///
/// \descr  This service returns the current application mode.
///
/// \param  void
///
/// \return OsAppModeType : The application mode
//------------------------------------------------------------------------------------------------------------------
OsAppModeType OS_GetActiveApplicationMode(void)
{
  return(APP_MODE);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ShutdownOS
///
/// \descr  The user can call this system service to abort the overall system (e.g. emergency off). 
///         The operating system also calls this function internally, 
///         if it has reached an undefined internal state and is no longer ready to run.
///
/// \param  OsStatusType Error: Error causing the shutdown of the OS
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ShutdownOS(OsStatusType Error)
{
#if(SHUTDOWNHOOK)
  ShutdownHook(Error);
#else
  (void)Error;
#endif

  DISABLE_INTERRUPTS();

  /* Kill all tasks */
  for(int tcbIdx = 0; tcbIdx < NB_OF_TASKS; tcbIdx++)
  {
    OCB_Cfg.pTcb[tcbIdx]->TaskStatus = SUSPENDED;
  }

  for(;;);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_IdleLoop
///
/// \descr  This function is called by the dispatcher when no active task is found.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void OS_IdleLoop(void)
{
  /* No active task found */
  for(;;);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsRunCat2Isr
///
/// \descr  This function is the entry point of all category 2 interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsRunCat2Isr(void);

void OsRunCat2Isr(void)
{
 /* Get program status register */
  uint32 CurrentPsr = OsGetCPSR();

  for(uint32 IsrIdx=0; IsrIdx < NB_OF_ISR; IsrIdx++)
  {
    if(IsrLookupTable[IsrIdx].IsrNum == (uint32)(CurrentPsr & EXCEPTION_CODE_MASK))
    {
      IsrLookupTable[IsrIdx].IsrFunc();
      return;
    }
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIsInterruptContext
///
/// \descr  This function check the cpu mode (thread or interrupt).
///
/// \param  void
///
/// \return boolean: TRUE -> Cpu in interrupt context, FALSE -> Cpu not in interrupt context
//------------------------------------------------------------------------------------------------------------------
boolean OsIsInterruptContext(void)
{
  uint32 CurrentPsr = OsGetCPSR();

  return((((CurrentPsr & CPU_MODE) == 0x11) || ((CurrentPsr & CPU_MODE) == 0x12)) ? TRUE : FALSE);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIsCat2IntContext
///
/// \descr  This function check if the cpu is executing a category 2 interrupt.
///
/// \param  void
///
/// \return boolean: TRUE -> Category 2 interrupt context, FALSE -> none category 2 interrupt context
//------------------------------------------------------------------------------------------------------------------
boolean OsIsCat2IntContext(void)
{
  return((boolean)OCB_Cfg.OsIsrInterruptLevel);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_DisableAllInterrupts
///
/// \descr  Disable all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_DisableAllInterrupts(void)
{
  DISABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_EnableAllInterrupts
///
/// \descr  enable all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_EnableAllInterrupts(void)
{
  ENABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SuspendAllInterrupts
///
/// \descr  Suspend all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_SuspendAllInterrupts(void)
{
  DISABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ResumeAllInterrupts
///
/// \descr  Resume all suspended interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ResumeAllInterrupts(void)
{
  ENABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SuspendOSInterrupts
///
/// \descr  Suspend all OS interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_SuspendOSInterrupts(void)
{
  DISABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ResumeOSInterrupts
///
/// \descr  Resume all suspended OS interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ResumeOSInterrupts(void)
{
  ENABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsUndefinedIsr
///
/// \descr  default handler for EI level interrupt
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsUndefinedIsr(void);

void OsUndefinedIsr(void)
{
  #if(ERRORHOOK)
  ErrorHook(E_OS_DISABLEDINT);
  #endif
  for(;;);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsFeUndefinedIsr
///
/// \descr  default handler for FE level interrupt
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsFeUndefinedIsr(void);

void OsFeUndefinedIsr(void)
{
  #if(ERRORHOOK)
  ErrorHook(E_OS_DISABLEDINT);
  #endif
  for(;;);
}

//-----------------------------------------------------------------------------
/// \brief  OsGetSystemTicksCounter
///
/// \descr  Get the system tick counter
///
/// \param  void
///
/// \return uint64 
//-----------------------------------------------------------------------------
uint64 OsGetSystemTicksCounter(void)
{
  return(OCB_Cfg.OsSysTickCounter);
}

//-----------------------------------------------------------------------------
/// \brief  OsGetSystemTicksElapsedTime
///
/// \descr  Get the system elapsed time relative to the prvTicks
///
/// \param  prvTicks : perivous system tick
///
/// \return uint64 
//-----------------------------------------------------------------------------
uint64 OsGetSystemTicksElapsedTime(uint64 prvTicks)
{
  return((uint64)(OCB_Cfg.OsSysTickCounter - prvTicks));
}

//-----------------------------------------------------------------------------
/// \brief  osErrTaskExitWithoutTerminate
///
/// \descr  Catch abnormal exit from task function
///
/// \param  void 
///
/// \return void 
//-----------------------------------------------------------------------------
void osErrTaskExitWithoutTerminate(void)
{
  DISABLE_INTERRUPTS();
  for(;;);
}
