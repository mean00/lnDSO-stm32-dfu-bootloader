

#include <string.h>
#include "lnArduino.h"
#include "lnRCU.h"
#include "mini8bit.h"
#include "lnIRQ.h"
extern const uint8_t dso_resetOff[] ;
extern const uint8_t dso_wakeOn[] ;

#define configTICK_RATE_HZ                      1000
#define configSYSTICK_CLOCK_HZ                  (72*1000*1000)

#define portNVIC_SYSTICK_CTRL_REG             ( *( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG             ( *( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG    ( *( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SHPR3_REG                    ( *( ( volatile uint32_t * ) 0xe000ed20 ) )
#define portNVIC_SYSTICK_CLK_BIT              ( 1UL << 2UL )
#define portNVIC_SYSTICK_INT_BIT              ( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT           ( 1UL << 0UL )

/**
 * 
 */
void lcdRun()
{

   /* Stop and clear the SysTick. */
    portNVIC_SYSTICK_CTRL_REG = 0UL;
    portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;

    /* Configure SysTick to interrupt at the requested rate. */
    portNVIC_SYSTICK_LOAD_REG = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
    portNVIC_SYSTICK_CTRL_REG = ( portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT );


    // Activate GPIO A,B,C
    lnPeripherals::enable(pGPIOA);
    lnPeripherals::enable(pGPIOB);
    lnPeripherals::enable(pGPIOC);

	ln8bit9341 ili( 240, 320,
                    1,          // port B
                    PC14,  //DC/RS
                    PC13, // CS
                    PC15, // WRITE
                    PA6,  // READ
                    PB9 ); // RESET                                  
    ili.init(dso_resetOff,dso_wakeOn);    
    ili.setRotation(1);
    ili.fillScreen(0x10f);


    portNVIC_SYSTICK_CTRL_REG=portNVIC_SYSTICK_CLK_BIT+portNVIC_SYSTICK_ENABLE_BIT;
    lnPeripherals::disable(pGPIOA);
    lnPeripherals::disable(pGPIOB);
    lnPeripherals::disable(pGPIOC);
}
// EOF