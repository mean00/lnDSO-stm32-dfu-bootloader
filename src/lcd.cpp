

#include <string.h>
#include "lnArduino.h"
#include "lnRCU.h"
//#include "mini8bit.h"
#include "lnIRQ.h"
//extern const uint8_t dso_resetOff[] ;
//extern const uint8_t dso_wakeOn[] ;

int start,end;
extern volatile int sysTick;
/**
 * 
 */
#if 1
extern "C"
{
#include "ili9341_stm32_parallel8.h"
}
void runLcd()
{
    ili_init();
    ili_rotate_display(1);
	  ili_fill_screen(ILI_COLOR_CYAN);
  while(1)
  {
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
  }
}
#else
void runLcd()
{
   
	ln8bit9341 ili( 240, 320,
                    1,          // port B
                    PC14,  //DC/RS
                    PC13, // CS
                    PC15, // WRITE
                    PA6,  // READ
                    PB9 ); // RESET                                  
    ili.init(dso_resetOff,dso_wakeOn);    
  //  ili.setRotation(1);
  while(1)
  {
    ili.fillScreen(0x5a);
  }
}
#endif
// EOF