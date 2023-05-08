// *****************************************************************************
// Filename    : OsTypes.h
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
// Description : Os types definition header file
// 
// *****************************************************************************

#ifndef __OS_TYPES_H__
#define __OS_TYPES_H__

#include"Platform_Types.h"
#include"HwPltfm.h"

typedef void (*pTaskFunc)(void);

typedef enum
{
  PRE_READY,
  READY,
  WAITING,
  RUNNING,
  SUSPENDED,
  INVALID_STATE
}OsTaskStateType;


typedef enum
{
  BASIC,
  EXTENDED
}OsTasksType;

typedef enum
{
  NONE_PREEMPT,
  FULL_PREEMPT
}OsTasksSchedType;


typedef enum
{
  E_OK                       = 0,
  E_OS_ACCESS                = 1,
  E_OS_CALLEVEL              = 2,
  E_OS_ID                    = 3,
  E_OS_LIMIT                 = 4,
  E_OS_NOFUNC                = 5,
  E_OS_RESOURCE              = 6,
  E_OS_STATE                 = 7,
  E_OS_VALUE                 = 8,
  E_OS_SERVICEID             = 9,
  E_OS_ILLEGAL_ADDRESS       = 10,
  E_OS_MISSINGEND            = 11,
  E_OS_DISABLEDINT           = 12,
  E_OS_STACKFAULT            = 13,
  E_OS_PROTECTION_MEMORY     = 14,
  E_OS_PROTECTION_TIME       = 15,
  E_OS_PROTECTION_ARRIVAL    = 16,
  E_OS_PROTECTION_LOCKED     = 17,
  E_OS_PROTECTION_EXCEPTION  = 18,
  E_OS_SYS_ASSERTION         = 20,
  E_OS_SYS_ABORT             = 21,
  E_OS_SYS_API_ERROR         = 23,
  E_OS_SYS_ALARM_MANAGEMENT  = 24,
  E_OS_SYS_WARNING           = 25
}OsStatusType;

typedef enum
{
  ALARM_SET_EVENT,
  ALARM_ACTIVE_TASK,
  ALARM_CALLBACK
  
}OsAlarmAction;

typedef enum
{
  ALARM_FREE,
  ALARM_USED
}AlarmStatus;

typedef enum
{
  ALARM_ONE_SHOT,
  ALARM_CYCLIC
}AlarmTypes;

typedef enum
{
  ALARM_RELATIVE,
  ALARM_ABSOLUTE
}AlarmCatgys;

/* OS TYPES */
typedef unsigned int OsTaskType;
typedef unsigned int OsEventMaskType;
typedef OsEventMaskType* OsEventMaskRefType;
typedef OsTaskType* OsTaskRefType; 
typedef OsTaskStateType* OsTaskStateRefType;
typedef unsigned int OsResourceType;
typedef unsigned int OsAppModeType;
typedef unsigned int OsAlarmType;
typedef unsigned int OsTickType;
typedef OsTickType* OsTickRefType;

/* TCB & CCB typedef */

typedef struct
{
  volatile const pTaskFunc   function;
  volatile const uint32      FixedPrio;
  volatile const uint32      Autostart;
  volatile const uint32      pstack_top;
  volatile const uint32      pstack_bot;
  volatile uint32            pCurrentStackPointer;
  volatile OsTaskStateType   TaskStatus;
  volatile uint32            Prio;
  volatile uint32            CeilingPrio;
  volatile uint32            SetEvtMask;
  volatile uint32            WaitEvtMask;
  volatile uint32            NbOfActiv;
  volatile OsTasksType       TaskType;
  volatile OsTasksSchedType  TaskSchedType;
}Tcb_t;

typedef struct
{
  volatile OsTickType             InitTicks;
  volatile OsTickType             InitCycles;
  const OsEventMaskType           Event;
  const OsTaskType                TaskId;
  const OsAlarmAction             Action;
  const uint32                    AutoStart;
  volatile AlarmStatus            Status;
  volatile AlarmTypes             Alarmtype;
  volatile AlarmCatgys            AlarmCategory;
  volatile OsTickType             AlarmCheckPoint;
  volatile pTaskFunc              CallBackFunc;
}Alarm_t;

typedef struct
{
  const uint32 ResCeilingPrio;
  uint32 CurrentOccupiedTask;
  const uint32* const AuthorizedTask;
}Resource_t;

typedef Alarm_t** OsAlarmBaseRefType;

typedef struct
{
           Tcb_t**       pTcb;
           Alarm_t**     pAlarm;
           Resource_t**  pRes;
  volatile uint32        OsIsRunning;
  volatile uint32        HighPrioReadyTaskIdx;
  volatile uint32        CurrentTaskIdx;
  volatile uint64        OsSysTickCounter;
  volatile uint32        OsIsrCallDispatcher;
  volatile uint32        OsLockDispatcher;
  volatile uint32        OsIsrInterruptLevel;
  volatile uint32        OsCurrentSystemStackPtr;
}Ocb_t;

typedef struct
{
  uint32 IsrNum;
  pFunc  IsrFunc;
}Isr_t;


typedef struct
{
  pFunc  IsrFunc;
}FeIsr_t;

#define TASK(x) void Task##x##Func(void); \
                void Task##x##Func(void)
#define ISR(x)  void Os##x##Isr(void)
#define ALARMCALLBACK(x) void OsAlarmCallback##x##Func(void)
#define pTASK(x) (const pTaskFunc)&Task##x##Func

#define STACK(x,y) uint32 Stack_T_##x[(y/4)]; const uint32 Stack_T_##x##_Size = ((y / 4) * 4)
#define TSTACK(x,y) (uint32)(&Stack_T_##x[(y/4)-1])
#define BSTACK(x)   (uint32)(&Stack_T_##x[0])

#define OS_DeclareTask(TaskId)
#define OS_DeclareResource(ResId)
#define OS_DeclareEvent(Event)
#define OS_DeclareAlarm(AlarmId)

#define OS_AUTOSTART    (uint32)1
#define OS_NO_AUTOSTART (uint32)0

#define ALARM_AUTOSTART    (uint32)1
#define ALARM_NO_AUTOSTART (uint32)0

#define APP_MODE_DEFAULT (OsAppModeType)0
#define APP_MODE APP_MODE_DEFAULT

#define OS_FALSE 0U
#define OS_TRUE  1U

#endif

