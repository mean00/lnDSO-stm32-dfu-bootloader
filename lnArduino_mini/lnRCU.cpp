/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnRCU.h"
#include "lnRCU_priv.h"
#include "lnPeripheral_priv.h"
LN_RCU *arcu=(LN_RCU *)LN_RCU_ADR;
/**
 */

/**
 *
 * @param periph
 */
void lnPeripherals::enable(const Peripherals periph)
{
    uint32_t bit=0;
    switch(periph)
    {
        case pAF:    bit=LN_RCU_APB2_AFEN;break;     
        case pGPIOA: bit=LN_RCU_APB2_PAEN;break;
        case pGPIOB: bit=LN_RCU_APB2_PBEN;break;
        case pGPIOC: bit=LN_RCU_APB2_PCEN;break;
        default: xAssert(0);break;
    }
    arcu->APB2EN |=bit;
}
 // EOF
