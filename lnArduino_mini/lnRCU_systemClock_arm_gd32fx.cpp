/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#include "lnArduino.h"
#include "lnRCU.h"
#include "lnRCU_priv.h"
#include "lnCpuID.h"

extern LN_RCU *arcu;
//

uint32_t _rcuClockApb1=108000000/2;
uint32_t _rcuClockApb2=108000000;
extern "C" uint32_t SystemCoreClock;
uint32_t SystemCoreClock=0;
uint32_t jtagId=0;
/**
 *
 * @param periph
 * @return
 */
uint32_t lnPeripherals::getClock(const Peripherals periph)
{
    switch(periph)
    {        
        case pSYSCLOCK:
                    return SystemCoreClock;break;
        default:
                    return _rcuClockApb1;break;
    }
}


//
#define LN_CLOCK_IRC8  0
#define LN_CLOCK_XTAL  16
#define LN_CLOCK_PLL   24
static void enableDisableClock(int clock, bool enabled)
{
   

}
/**
 *
 */
void lnInitSystemClock()
{
 
}
// EOF
