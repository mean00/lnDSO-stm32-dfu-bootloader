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

static uint32_t interruptVector[LN_NB_INTERRUPT]  __attribute__((aligned(256)));

extern "C" void xPortPendSVHandler();
extern "C" void xPortSysTickHandler();
extern "C" void vPortSVCHandler();
extern "C" void __exc_reset();

static  volatile uint32_t curInterrupt;
static  volatile LnIRQ   curLnInterrupt;

volatile LN_NVIC *anvic=(LN_NVIC *)0xE000E100;

#define LN_MSP_SIZE_UINT32  128
static uint32_t mspStack[LN_MSP_SIZE_UINT32]  __attribute__((aligned(8)));  // 128*4=512 bytes for msp

lnInterruptHandler *adcIrqHandler=NULL;
/**
 * \fn unsupportedInterrupt
 */

static void unsupportedInterrupt() LN_INTERRUPT_TYPE;
static void unsupportedInterrupt2() LN_INTERRUPT_TYPE;
void unsupportedInterrupt() 
{
    curInterrupt=aSCB->ICSR &0xff;
    curLnInterrupt=(LnIRQ)(curInterrupt-LN_VECTOR_OFFSET);
    __asm__  ("bkpt 1");  
    xAssert(0);
}
void unsupportedInterrupt2() 
{
    __asm__  ("bkpt 1");  
    curInterrupt=aSCB->ICSR &0xff;
    curLnInterrupt=(LnIRQ)(curInterrupt-LN_VECTOR_OFFSET);
    
    xAssert(0);
}
/**
 *  \brief SoftReset, only resets the core
 * */
void lnSoftSystemReset()
{
    uint32_t aircr=aSCB->AIRCR;
    aircr= AIRCR_KEY+(1<<0); // VECTRESET
    aSCB->VTOR=0; // boot to bootloader, not the app
    aSCB->AIRCR= aircr;
    while(1)
    {

    }
}

/**
 * \brief Full reset, reset the full MCU
 */
void lnHardSystemReset()
{
    uint32_t aircr=aSCB->AIRCR;
    aircr= AIRCR_KEY+(1<<2); // SYSREQRESET
    aSCB->VTOR=0; // boot to bootloader, not the app
    aSCB->AIRCR= aircr;
    while(1)
    {

    }
}
#if 0
/**
 * 
 * @param irq
 * @param prio
 */
void lnIrqSetPriority(const LnIRQ &irq, int prio )
{
    // Interrupt set should be between configLIBRARY_LOWEST_INTERRUPT_PRIORITY<<4 and configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY<<4
    // Low interrupt means more urgent
    prio=configLIBRARY_LOWEST_INTERRUPT_PRIORITY-prio;
    if(prio>configLIBRARY_LOWEST_INTERRUPT_PRIORITY) prio=configLIBRARY_LOWEST_INTERRUPT_PRIORITY;
    if(prio<configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY) prio=configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
    
    if(irq<LN_IRQ_WWDG) // Non IRQ
    {  
        return;
    }
    anvic->IP[irq]=prio<<(8-configPRIO_BITS)   ;
}
#endif

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
 * @param irq
 * @param handler
 */
void lnSetInterruptHandler(const LnIRQ &irq, lnInterruptHandler*handler)
{
    switch(irq)
    {
        case LN_IRQ_ADC0_1: adcIrqHandler=handler;break;
        default:
            xAssert(0);
            break;
    }
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
