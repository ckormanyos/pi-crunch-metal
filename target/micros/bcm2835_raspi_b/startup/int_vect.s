// ***************************************************************************************
// Filename    : IntVectTable.s
// 
// Core        : ARM1176JZF-S
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 11.03.2020
// 
// Description : Interrupt vector table definition for ARM1176JZF-S(ARMv6)
// 
// ***************************************************************************************

.section .intvect, "ax"
  b reset                              // reset               Reset
  b __int_vect_undef_instr_handler     // undefined_handler   Undefined instructions
  b OsDispatcher                       // OsDispatcher        Software interrupt (SWI)
  b __int_vect_prefetch_abort_handler  // prefetch_handler    Prefetch Abort (instruction fetch memory abort)
  b __int_vect_data_abort_handler      // data_handler        Data Abort (data access memory abort)
  b __int_vect_unused_05_handler       // unused_handler      Not used
  b OsCat2IsrIrqWrapper                // OsCat2IsrIrqWrapper IRQ (interrupt)
  b OsCat1Isr                          // OsCat1Isr           FIQ (fast interrupt)

.extern __my_startup

.section .reset, "ax"
.globl reset
.arm

reset:

  mov r14, #0
  mov r12, #0
  mov r11, #0
  mov r10, #0
  mov r9,  #0
  mov r8,  #0
  mov r7,  #0
  mov r6,  #0
  mov r5,  #0
  mov r4,  #0
  mov r3,  #0
  mov r2,  #0
  mov r1,  #0
  mov r0,  #0

  /* The ARM1176JZF-S is started in supervisor mode with interrupt disabled */
  /* Setup the stack pointer for the supervisor mode */

  ldr r3, =__SVC_STACK_TOP
  mov sp, r3

  /* relocate the interrupt vector table */
  ldr r3, =__INTVECT_BASE_ADDRESS
  MCR p15, 0, r3, c12, c0, 0

  /* switch to Abort mode and setup the relevant stack pointer */
  cps #0x17
  ldr r3, =__ABT_STACK_TOP
  mov sp, r3

  /* switch to Undefined mode and setup the relevant stack pointer */
  cps #0x1b
  ldr r3, =__UND_STACK_TOP
  mov sp, r3

  /* switch to IRQ mode and setup the relevant stack pointer */
  cps #0x12
  ldr r3, =__IRQ_STACK_TOP
  mov sp, r3

  /* switch to FIQ mode and setup the relevant stack pointer */
  cps #0x11
  ldr r3, =__FIQ_STACK_TOP
  mov sp, r3

  /* switch to System mode and setup the relevant stack pointer */
  cps #0x1f
  ldr r3, =__SYS_STACK_TOP
  mov sp, r3

  /* Enable the Program flow prediction, I-Cache and D-Cache */
  mrc p15, 0, r1, c1, c0, 0
  ldr r0, =#0x1804
  orr r1, r0
  mcr p15, 0, r1, c1, c0, 0

  /* Enable the floating point unit vfp11 (ARMv6 coprocessor cp10 and cp11) */
  mrc p15, 0, r1, c1, c1, 2 //Secure or Non-secure access.
  ldr r0, =0x00000C00
  orr r1, r0
  mcr p15, 0, r1, c1, c1, 2


  mrc p15, 0, r1, c1, c0, 2 //Privileged and User mode access.
  ldr r0, =0x00F00000
  orr r1, r0
  mcr p15, 0, r1, c1, c0, 2

  ldr r0, =#0x40000000 //Enable FPU
  vmsr fpexc,r0

  /* Jump to the C-Runtime Startup */
  b __my_startup
