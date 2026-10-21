/* Editor-only ARMCC5 compatibility. Never include this in firmware sources. */
#ifndef ARMCC_INTELLISENSE_H
#define ARMCC_INTELLISENSE_H

#ifdef __INTELLISENSE__
/* MSVC IntelliSense cannot parse ARMCC attributes or named ARM registers. */
#define __attribute__(attributes)
#define __asm(register_name)

/* Use declarations for the two CMSIS functions implemented in ARM assembly. */
#define __NO_EMBEDDED_ASM
#include <stdint.h>
uint32_t __REV16(uint32_t value);
int32_t __REVSH(int32_t value);

/* ARMCC supplies these intrinsics without header declarations. */
void __enable_irq(void);
void __disable_irq(void);
void __nop(void);
void __wfi(void);
void __wfe(void);
void __sev(void);
void __isb(unsigned int option);
void __dsb(unsigned int option);
void __dmb(unsigned int option);
uint32_t __rev(uint32_t value);
uint32_t __ror(uint32_t value, uint32_t shift);
uint32_t __rbit(uint32_t value);
uint8_t __clz(uint32_t value);
#endif

#endif
