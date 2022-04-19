

#include <string.h>
#include "lnArduino.h"
#include "lnRCU.h"
#include "mini8bit.h"
#include "lnIRQ.h"
extern const uint8_t dso_resetOff[] ;
extern const uint8_t dso_wakeOn[] ;

/**
 * 
 */
void lcdRun()
{

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

}
// EOF