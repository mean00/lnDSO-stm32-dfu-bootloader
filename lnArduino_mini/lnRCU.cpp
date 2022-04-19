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
    {        pNONE,          0,          0},    
    {        pUSB,           1,          LN_RCU_APB1_USBDEN}, // not sure
    {        pGPIOA,         2,          LN_RCU_APB2_PAEN},
    {        pGPIOB,         2,          LN_RCU_APB2_PBEN},
    {        pGPIOC,         2,          LN_RCU_APB2_PCEN},
    {        pGPIOD,         2,          LN_RCU_APB2_PDEN},
    {        pGPIOE,         2,          LN_RCU_APB2_PEEN},
    {        pAF,            2,          LN_RCU_APB2_AFEN},
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
/**

*/
extern uint32_t _rcuClockApb1;
void lnPeripherals::enableUsb48Mhz()
{
  static bool usb48M=false;
  if(usb48M) return;
  usb48M=true;
  // careful, the usb clock must be off !
  int scaler=(2*lnPeripherals::getClock(pSYSCLOCK))/48000000;
  int x=0;

  switch(scaler)
  {
      case 3: x=0;break; // 3/2=1.5
      case 2: x=1;break; // 2/2=1
      case 5: x=2;break; // 5/2=2.5
      case 4: x=3;break; // 4/2=2
      default:
        xAssert(0); // invalid sys clock
        break;
  }

  if(lnCpuID::vendor()!=lnCpuID::LN_MCU_GD32)
  {
    if(x>1) xAssert(0); // STM32F1 chip only supports div by 1 and div by 1.5, i.e. x=0 or 1
                        // only GD32 has more dividers
  }
  uint32_t cfg0=arcu->CFG0;
  cfg0&=LN_RCU_CFG0_USBPSC_MASK;
  cfg0|=LN_RCU_CFG0_USBPSC(x);
  arcu->CFG0=cfg0;
}
/**
 *
 * @param divider
 */
void lnPeripherals::setAdcDivider(lnADC_DIVIDER divider)
{
    uint32_t val=arcu->CFG0;

    val&=LN_RCU_ADC_PRESCALER_MASK;
    int r=(int)divider;
    if(r&4)
    {
        if(lnCpuID::vendor()==lnCpuID::LN_MCU_GD32) // only up to 8
        {
          val|=LN_RCU_ADC_PRESCALER_HIGHBIT;
          val|=LN_RCU_ADC_PRESCALER_LOWBIT(r&3);
        }
        else
            val |=LN_RCU_ADC_PRESCALER_LOWBIT(lnADC_CLOCK_DIV_BY_8);
    }
    else
    {
        val|=LN_RCU_ADC_PRESCALER_LOWBIT(r);
    }
    arcu->CFG0=val;

}
// EOF
