
  PUBLIC mcal_reg_access32_write
  PUBLIC mcal_reg_access32_read

  SECTION .text:CODE

mcal_reg_access32_write
  mov r2, #0
  mcr p15, 0, r2, c7, c10, 5
  str r1, [r0]
  bx lr

mcal_reg_access32_read
  mov r2, #0
  ldr r0, [r0]
  mcr p15, 0, r2, c7, c10, 5
  bx lr

  END
