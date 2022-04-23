

#include <string.h>
#include "lnArduino.h"
#include "lnRCU.h"
#include "lnIRQ.h"
#include "ili_hs.h"
#include "ili9341_stm32_parallel8.h"
#include "../gfx/generated/dfu_usb_decl.h"
#include "../gfx/generated/dfu_usb_compressed.h"
#define ST7735_BUFFER_SIZE_WORD 128
void drawHSBitmap(int widthInPixel, int height, int wx, int wy, int fgcolor, int bgcolor, const uint8_t *data)
{
    iliHS hs(data);
    int nbPixel=widthInPixel*height;
    int pixel=0;
    ili_set_address_window(wx, wy,  widthInPixel+wx-1, height+height-1);
    int mask=0;
    int cur;
    uint16_t temp[ST7735_BUFFER_SIZE_WORD];
    uint16_t *o=temp;
    int ready=0;

    uint16_t color;
    ili_dataBegin();
    while(pixel<nbPixel)
    {
        // load next
        cur= hs.next();
        int mask=0x80;
        for(int i=0;i<8;i++)
        {
            if(mask & cur)
            {
                color=fgcolor;
            }else
                color=bgcolor;
            mask>>=1;
            *o++=color;
            ready++;
        }
        if(ready>(ST7735_BUFFER_SIZE_WORD-16))
        { // Flush
          ili_sendWords(ready,temp);
          ready=0;
          o=temp;
        }
        pixel+=8;
    }
    if(ready)
        ili_sendWords(ready,temp);
    ili_dataEnd();
}

/**
 * 
 */
const uint8_t data[]={0};
void runLcd()
{
  ili_init();
  ili_rotate_display(1);
  ili_fill_screen(ILI_COLOR_BLACK);

  drawHSBitmap(dfu_usb_width,dfu_usb_height,(320-dfu_usb_width)/2,(240-dfu_usb_height)/2,
        0xffff,0,
        dfu_usb);

}

// EOF