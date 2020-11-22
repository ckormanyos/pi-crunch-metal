#include <mcal_reg.h>

void mcal_reg_access32_write(const uint32_t address, const uint32_t value)
{
  asm volatile("mov r2, #0");
  asm volatile("mcr p15, 0, r2, c7, c10, 5");
  asm volatile("str r1, [r0]");
  asm volatile("bx lr");
}

uint32_t mcal_reg_access32_read (const uint32_t address)
{
  asm volatile("mov r2, #0");
  asm volatile("ldr r0, [r0]");
  asm volatile("mcr p15, 0, r2, c7, c10, 5");
  asm volatile("bx lr");
}