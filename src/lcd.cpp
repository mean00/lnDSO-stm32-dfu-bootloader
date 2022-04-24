

#include <string.h>
#include "lnArduino.h"
#include "lnRCU.h"
#include "lnIRQ.h"
#include "ili_hs.h"
#include "ili9341_stm32_parallel8.h"
#include "../gfx/generated/dfu_usb_decl.h"
#include "../gfx/generated/dfu_usb_compressed.h"

#define ST7735_BUFFER_SIZE_WORD 128

void drawHSBitmap(int widthInPixel, int height, int wx, int wy, const uint8_t *data)
{
    iliHS hs(data);
    int nbPixel=widthInPixel*height;
    int pixel=0;
    
    int mask=0;
    int cur;
    uint16_t temp[ST7735_BUFFER_SIZE_WORD];
    uint16_t *o=temp;
    int ready=0;

    uint16_t color;

    ili_set_address_window(wx, wy,  widthInPixel+wx-1, height+wy-1);
    ili_dataBegin();
    while(pixel<nbPixel)
    {
        cur= hs.next();
        int mask=0x80;
        for(int i=0;i<8;i++)
        {
            if(mask & cur)
            {
              *o++=0xffff;
            }else
            {
              *o++=0;
            }
            mask>>=1;            
        }
        ready+=8;
        pixel+=8;
        if(ready>(ST7735_BUFFER_SIZE_WORD-16))
        { // Flush
          ili_sendWords(ready,temp);
          ready=0;
          o=temp;
        }        
    }
    if(ready)
        ili_sendWords(ready,temp);
    ili_dataEnd();
}

/**
 * 
 */

void runLcd()
{
  ili_init();
  ili_rotate_display(1);
  ili_fill_screen(ILI_COLOR_BLACK);

  drawHSBitmap(dfu_usb_width,dfu_usb_height,(320-dfu_usb_width)/2,48, dfu_usb);

}

#include "../gfx/generated/anim_0_compressed.h"
//#include "../gfx/generated/anim_2_compressed.h"
#include "../gfx/generated/anim_4_compressed.h"
//#include "../gfx/generated/anim_6_compressed.h"
#include "../gfx/generated/anim_8_compressed.h"
//#include "../gfx/generated/anim_10_compressed.h"
#include "../gfx/generated/anim_12_compressed.h"
//#include "../gfx/generated/anim_14_compressed.h"
#include "../gfx/generated/anim_16_compressed.h"
//#include "../gfx/generated/anim_18_compressed.h"
#include "../gfx/generated/anim_20_compressed.h"
//#include "../gfx/generated/anim_22_compressed.h"
#include "../gfx/generated/anim_0_decl.h"

/*
#define anim_0_width 55
#define anim_0_height 36
extern const unsigned char anim_0[];

*/

const uint8_t *animation_step[]=
{
#if 1
  #define NB_STEP 6
   anim_0,     anim_4,     anim_8,
   anim_12,    anim_16,    anim_20,   
#else
    #if 1
    #define NB_STEP 12
    anim_0,   anim_2,   anim_4,   anim_6,  anim_8,
    anim_10,  anim_12,  anim_14,  anim_16, anim_18,
    anim_20,  anim_22, 
    #else
    #define NB_STEP 24
    anim_0, anim_1, anim_2, anim_3, anim_4, anim_5, anim_6,anim_7,anim_8,anim_9,
    anim_10, anim_11, anim_12, anim_13, anim_14, anim_15, anim_16,anim_17,anim_18,anim_19,
    anim_20, anim_21, anim_22, anim_23
    #endif
#endif

};

int step=0;
void animate()
{
  drawHSBitmap( anim_0_width,anim_0_height,
                (320-anim_0_width)/2,140,
                animation_step[step]);
  step=step+1;
  if(step>=NB_STEP) step=0;
}

// EOF
