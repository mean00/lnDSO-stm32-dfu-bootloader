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
		if(sysTick>tgt) 
			return;
	}
}
extern "C" void delay(int ms)
{
    xDelay(ms);
}
/**
 * 
 */
void lnDelayUs(int us)
{
	int count=(us*72)/6; // appromixate
	for(int i=0;i<count;i++)
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


extern "C" void _write() { xAssert(0);}
extern "C" void *_sbrk(ptrdiff_t incr) { xAssert(0);return 0;}
extern "C" void *sbrk(ptrdiff_t incr) { xAssert(0);return 0;}
extern "C" void _putchar(char) {};

#undef printf
#undef scanf

extern "C" int	printf (const char *__restrict, ...) {return 0;}
extern "C" int	scanf (const char *__restrict, ...) {return 0;}

/**

*/
#if 0
void lnDelayUs(int wait)
{
    uint64_t target=lnGetUs()+wait;
    while(1)
    {
        uint64_t vw=lnGetUs();
        if(vw>target)
            return;
        __asm__("nop"::);
    }

}

#endif

// EOF