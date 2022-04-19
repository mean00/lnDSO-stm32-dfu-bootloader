#include "lnArduino.h"
#include "lnIRQ.h"
#include "lnIRQ_arm_priv.h"
#include "lnSCB_arm_priv.h"
#include "lnNVIC_arm_priv.h"
#include "lnIRQ_arm.h"
#include "sys/types.h"


volatile uint32_t sysTick=0;

extern "C" void tick(void)
{
	sysTick++;
}

extern "C" void do_assert(const char *msg)
{
    deadEnd(2);
}

void xDelay(int ms)
{
	uint32_t tgt=sysTick+ms;
	while(1)
	{
		if(sysTick>tgt) return;
	}
}
void delay(int ms)
{
    xDelay(ms);
}
void lnDelayUs(int us)
{

	for(int i=0;i<us;i++)
		for(int j=0;j<72;j++)
				__asm__("nop");

}

extern "C" void deadEnd(int z)
{
    while(1){}
}


// Implement this here to save space, quite minimalistic :D
void *memcpy(void * dst, const void * src, size_t count) {
	uint8_t * dstb = (uint8_t*)dst;
	uint8_t * srcb = (uint8_t*)src;
	while (count--)
		*dstb++ = *srcb++;
	return dst;
}

