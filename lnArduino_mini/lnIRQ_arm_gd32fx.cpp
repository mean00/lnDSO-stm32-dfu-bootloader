/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */

#include "lnArduino.h"
#include "lnRCU.h"
#include "lnIRQ.h"
#include "lnIRQ_arm_priv.h"
#include "lnSCB_arm_priv.h"
#include "lnNVIC_arm_priv.h"
#include "lnIRQ_arm.h"

/**
 * 
 */

#define AIRCR_KEY (0x5FA<<16)

#define LN_NB_INTERRUPT 68
#define LN_VECTOR_OFFSET 16

LN_SCB_Registers *aSCB  LN_USED =(LN_SCB_Registers *)0xE000ED00;

uint32_t *armCurrentInterrupt   LN_USED =(uint32_t *)0xE000ED04;
uint32_t *armFaultStatusRegister  LN_USED =(uint32_t *)0xE000ED28;


volatile LN_NVIC *anvic=(LN_NVIC *)0xE000E100;




/**
 *  \brief SoftReset, only resets the core
 * */
static void sysReset(int x)
{
    uint32_t aircr=aSCB->AIRCR;
    aircr= AIRCR_KEY+(1<<x); // VECTRESET
    aSCB->VTOR=0; // boot to bootloader, not the app
    aSCB->AIRCR= aircr;
    while(1)
    {

    }
}

void lnSoftSystemReset()
{
    sysReset(0);
}

/**
 * \brief Full reset, reset the full MCU
 */
void lnHardSystemReset()
{
    sysReset(2);
}


/**
 * 
 * @param enableDisable
 * @param zirq
 */
void _enableDisable(bool enableDisable, const LnIRQ &zirq)
{
    int irq=(int)zirq;
    
    xAssert(irq>0) ;
    
    int offset=irq/32;
    int bit=irq&31;
    if(enableDisable)
    {
        anvic->ISER.data[offset]=1<<bit;
    }else
    {
        anvic->ICER.data[offset]=1<<bit;
    }
}
/**
 * 
 * @param per
 */
void lnEnableInterrupt(const LnIRQ &irq)
{   
    _enableDisable(true,irq);   
}

/**
 * 
 * @param per
 */
void lnDisableInterrupt(const LnIRQ &irq)
{
     _enableDisable(false,irq);
}
// EOF
