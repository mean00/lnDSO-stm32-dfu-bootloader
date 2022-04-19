/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnRCU.h"
#include "lnRCU_priv.h"
#include "lnPeripheral_priv.h"
#include "lnCpuID.h"
LN_RCU *arcu=(LN_RCU *)LN_RCU_ADR;
/**
 */
struct RCU_Peripheral
{
    uint8_t                     periph;
    uint8_t                     AHB_APB; // 1=APB 1, 2=APB2,8=AHB
    uint32_t                    enable;
};

#define RCU_RESET   1
#define RCU_ENABLE  2
#define RCU_DISABLE 3

/**
 */
static const RCU_Peripheral _peripherals[]=
{           // PERIP          APB         BIT

    {        pNONE,          2,          0},
    {        pGPIOA,         2,          LN_RCU_APB2_PAEN},
    {        pGPIOB,         2,          LN_RCU_APB2_PBEN},
    {        pGPIOC,         2,          LN_RCU_APB2_PCEN},
    //
    //
};

// 1 : Reset
// 2: Enable
// 3: disable
static void _rcuAction(const Peripherals periph, int action)
{
    xAssert((int)periph<100);
    RCU_Peripheral *o=(RCU_Peripheral *)(_peripherals+(int)periph);
    xAssert(o->periph==periph);
    xAssert(o->enable);
    switch(o->AHB_APB)
    {
        case 1: // APB1
            switch(action)
            {
                case RCU_RESET:
                        arcu->APB1RST|= o->enable;
                        arcu->APB1RST&= ~(o->enable); // not sure if it auto clears itself
                        break;
                case RCU_ENABLE: arcu->APB1EN |= o->enable;break;
                case RCU_DISABLE: arcu->APB1EN &=~o->enable;break;
                default : xAssert(0);break;
            }
            break;
        case 2: // APB2
            switch(action)
            {
                case RCU_RESET:
                        arcu->APB2RST|= o->enable;
                        arcu->APB2RST&=~( o->enable); // not sure if it auto clears itself
                        break;
                case RCU_ENABLE: arcu->APB2EN |= o->enable;break;
                case RCU_DISABLE: arcu->APB2EN &=~o->enable;break;
                default : xAssert(0);break;
            }
            break;
        case 8: // AHB
            switch(action)
            {
                case RCU_RESET:
                        // We can only reset USB
                      //  if(periph==pUSB) xAssert(0);
                        // else just ignore
                        break;
                case RCU_ENABLE: arcu->AHBEN |= o->enable;break;
                case RCU_DISABLE: arcu->AHBEN &=~o->enable;break;
                default : xAssert(0);break;
            }

            break;
        default:
            xAssert(0);
    }
}

/**
 *
 * @param periph
 */
void lnPeripherals::reset(const Peripherals periph)
{
    _rcuAction(periph,RCU_RESET);
}
/**
 *
 * @param periph
 */
void lnPeripherals::enable(const Peripherals periph)
{
    _rcuAction(periph,RCU_ENABLE);
}
/**
 *
 * @param periph
 */
void lnPeripherals::disable(const Peripherals periph)
{
    _rcuAction(periph,RCU_DISABLE);
}

// EOF
