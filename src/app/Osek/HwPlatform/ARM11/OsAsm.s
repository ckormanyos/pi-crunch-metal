// *****************************************************************************************************************
// Filename    : OsAsm.s
// 
// Core        : ARM1176JZF-S(ARMv6)
//
// Compiler    : ARM GCC
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 15.03.2020
// 
// Description : OSEK/ECC1 Context switch and ISR category 2 wrapper for ARM1176JZF-S(ARMv6)
// 
// *****************************************************************************************************************
  .syntax unified

  .section .text, "ax"

  .arm
//------------------------------------------------------------------------------------------------------------------
/// \brief  OsDispatcher
///
/// \descr  Context switcher
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
  .global OsDispatcher
  .align 4
  .code 32
  
  OsDispatcher:
  .extern OS_Dispatcher

  cps #0x1f                     /* Switch to user mode                                              */
  stmfd sp!, {r0-r12}           /* Save the context on the user stack                               */
  stmfd sp!, {lr}               /* Save the lr of the user mode on the user mode stack              */
  mov r0,sp                     /* prepare the input parameter for the function OS_Dispatcher       */
  cps #0x13                     /* Switch back to SVC mode                                          */
  stmfd r0!, {lr}               /* Save R14_SVC (user mode Return @) on the user stack              */
  mrs r1, spsr                  /* save the content of the spsr into r1                             */
  stmfd r0!, {r1}               /* Save SPSR_SVC (user mode CPSR) on the user stack                 */
  bl  OS_Dispatcher             /* Call the dispatcher to switch the context                        */
  mov sp, r0                    /* Use the returned stack pointer                                   */
  ldmfd sp!, {r1}               /* restore the saved cpsr register value                            */
  msr spsr, r1                  /* update the cpsr of the current mode                              */
  ldmfd sp!, {lr}               /* Restore R14_SVC (user mode Return @) register                    */
  ldmfd sp, {lr}^               /* Restore lr register                                              */
  add sp, sp, #4                /* adding 4 to the stack pointer value                              */
  ldmfd sp!, {r0-r12}           /* Restore the saved context                                        */
  stmfd sp!, {sp}               /* push sp to the stack in order to copy it to user area            */
  ldmfd sp, {sp}^               /* Pop the sp_usr value into the user stack pointer                 */
  ldr sp, =__SVC_STACK_TOP      /* Configure the stack pointer of the SVC mode for the next entry   */
  movs pc,lr                    /* Return from SVC exception with update of CPSR                    */


  //------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetCurrentSP : OsGetCurrentSP(volatile unsigned int* CurrentSpPtr)
///
/// \descr  Get the current stack pointer register value
///
/// \param  volatile unsigned int* CurrentSpPtr (out): Current stack pointer register value
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
 .global OsGetCurrentSP
 .align 4
 .arm

  bx lr

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetPSR : OsGetCPSR(void)
///
/// \descr  Get the program status register value
///
/// \param  void
///
/// \return current CPSR
//------------------------------------------------------------------------------------------------------------------
 .global OsGetCPSR
 .align 4
  .code 32

  OsGetCPSR:
  mrs r0, CPSR
  bx lr

  //------------------------------------------------------------------------------------------------------------------
/// \brief  OsCat2IsrIrqWrapper
///
/// \descr  Wrapper for all category 2 interrupts (IRQ)
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
  .global OsCat2IsrIrqWrapper
  .align 4
  .code 32

  OsCat2IsrIrqWrapper:

  .extern OsStoreStackPointer
  .extern OsGetSavedStackPointer
  .extern OsIsrCallDispatch
  .extern OsRunCat2Isr
  .extern OsSysTickTimerIsr

  cps #0x1f                     /* Switch to user mode                                              */
  stmfd sp!, {r0-r12}           /* Save the context on the user stack                               */
  stmfd sp!, {lr}               /* Save the lr of the user mode on the user mode stack              */
  mov r0,sp                     /* prepare the input parameter for the function OsStoreStackPointer */
  cps #0x12                     /* Switch back to IRQ mode                                          */
  mov r1, lr                    /* copy R14_irq into r1                                             */
  sub r1, r1, #4                /* subtract 4 from the value of the R14_irq because ARM11 added 4   */
  stmfd r0!, {r1}               /* Save R14_irq (user mode Return @) on the user stack              */
  mrs r1, spsr                  /* save the content of the spsr into r1                             */
  stmfd r0!, {r1}               /* Save SPSR_irq (user mode CPSR) on the user stack                 */
  bl OsStoreStackPointer        /* Save the stack pointer of the current task                       */
  bl OsSysTickTimerIsr          /* Call the ISR (lookup table)                                      */
  bl OsGetSavedStackPointer     /* Restore the stack pointer of the current task                    */
  bl OsIsrCallDispatch          /* Call dispatcher if needed                                        */
  mov sp, r0                    /* Use the returned stack pointer                                   */
  ldmfd sp!, {r1}               /* restore the saved cpsr register value                            */
  msr spsr, r1                  /* update the cpsr of the current mode                              */
  ldmfd sp!, {lr}               /* Restore R14_irq (user mode Return @) register                    */
  ldmfd sp, {lr}^               /* Restore lr register                                              */
  add sp, sp, #4                /* adding 4 to the stack pointer value                              */
  ldmfd sp!, {r0-r12}           /* Restore the saved context                                        */
  stmfd sp!, {sp}               /* push sp to the stack in order to copy it to user area            */
  ldmfd sp, {sp}^               /* Pop the sp_usr value into the user stack pointer                 */
  ldr sp, =__IRQ_STACK_TOP      /* Configure the stack pointer of the IRQ mode for the next entry   */
  movs pc,lr                    /* Return from exception with update of CPSR                        */

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsCat2IsrFiqWrapper
///
/// \descr  Wrapper for all category 2 interrupts (FIQ)
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
  .global OsCat2IsrFiqWrapper
  .align 4
  .code 32

  OsCat2IsrFiqWrapper:

  .extern OsStoreStackPointer
  .extern OsGetSavedStackPointer
  .extern OsIsrCallDispatch
  .extern OsRunCat2Isr

  cps #0x1f                     /* Switch to user mode                                              */
  stmfd sp!, {r0-r12}           /* Save the context on the user stack                               */
  stmfd sp!, {lr}               /* Save the lr of the user mode on the user mode stack              */
  mov r0,sp                     /* prepare the input parameter for the function OsStoreStackPointer */
  cps #0x11                     /* Switch back to FIQ mode                                          */
  mov r1, lr                    /* copy R14_irq into r1                                             */
  sub r1, r1, #4                /* subtract 4 from the value of the R14_irq because ARM11 added 4   */
  stmfd r0!, {r1}               /* Save R14_fiq (user mode Return @) on the user stack              */
  mrs r1, spsr                  /* save the content of the spsr into r1                             */
  stmfd r0!, {r1}               /* Save SPSR_fiq (user mode CPSR) on the user stack                 */
  bl OsStoreStackPointer        /* Save the stack pointer of the current task                       */
  bl OsSysTickTimerIsr          /* Call the system timer interrupt                                  */
  bl OsGetSavedStackPointer     /* Restore the stack pointer of the current task                    */
  bl OsIsrCallDispatch          /* Call dispatcher if needed                                        */
  mov sp, r0                    /* Use the returned stack pointer                                   */
  ldmfd sp!, {r1}               /* restore the saved cpsr register value                            */
  msr spsr, r1                  /* update the cpsr of the current mode                              */
  ldmfd sp!, {lr}               /* Restore R14_fiq (user mode Return @) register                    */
  ldmfd sp, {lr}^               /* Restore lr register                                              */
  add sp, sp, #4                /* adding 4 to the stack pointer value                              */
  ldmfd sp!, {r0-r12}           /* Restore the saved context                                        */
  stmfd sp!, {sp}               /* push sp to the stack in order to copy it to user area            */
  ldmfd sp, {sp}^               /* Pop the sp_usr value into the user stack pointer                 */
  ldr sp, =__FIQ_STACK_TOP      /* Configure the stack pointer of the FIQ mode for the next entry   */
  movs pc,lr                    /* Return from exception with update of CPSR                        */


//------------------------------------------------------------------------------------------------------------------
/// \brief  OsStartNewTask (OsStartNewTask(uint32 StackFramePtr, pFunc TaskFuncPtr))
///
/// \descr  This function starts an OSEK Task for the 1st execution
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
  .global OsStartNewTask
  .align 4
  .code 32

  OsStartNewTask:
  mov sp, r0                    /* Use the returned stack pointer                                   */
  ldmfd sp!, {r1}               /* Restore the CPSR                                                 */
  msr spsr_cxsf, r1             /* Update the spsr_svc register with the appropriet mode            */
  ldmfd sp!, {lr}               /* Restore R14_SVC (user mode Return @) register                    */
  ldmfd sp, {lr}^               /* Restore lr register                                              */
  add sp, sp, #4                /* adding 4 to the stack pointer value                              */
  ldmfd sp!, {r0-r12}           /* Restore the saved context                                        */
  stmfd sp!, {sp}               /* push sp to the stack in order to copy it to user area            */
  ldmfd sp, {sp}^               /* Pop the sp_usr value into the user stack pointer                 */
  ldr sp, =__SVC_STACK_TOP      /* Configure the stack pointer of the SVC mode for the next entry   */
  movs pc,lr                    /* Return from SVC exception with update of CPSR                    */

