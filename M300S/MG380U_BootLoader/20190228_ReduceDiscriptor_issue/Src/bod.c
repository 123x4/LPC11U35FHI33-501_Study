
#include "LPC11Uxx.h"

#include "bod.h" 

void bod_set_level( bod_rst_level_t rst_level, bod_int_level_t int_level )
{	
	LPC_SYSCON->BODCTRL = ((uint32_t) rst_level) | (((uint32_t) int_level) << 2);	
}


void bod_enable(void)
{
	LPC_SYSCON->BODCTRL |= (1 << 4);
	
}

void Clear_reset_status(uint32_t reset)
{
	LPC_SYSCON->SYSRSTSTAT = reset;
}
