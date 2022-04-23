/*
MIT License

Copyright (c) 2020 Avra Mitra

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#ifndef INC_ILI9341_STM32_PARALLEL8_H_
#define INC_ILI9341_STM32_PARALLEL8_H_

#define ILI_NOP     0x00
#define ILI_SWRESET 0x01
#define ILI_RDDID   0xD3
#define ILI_RDDST   0x09

#define ILI_SLPIN   0x10
#define ILI_SLPOUT  0x11
#define ILI_PTLON   0x12
#define ILI_NORON   0x13

#define ILI_RDMODE  0x0A
#define ILI_RDMADCTL  0x0B
#define ILI_RDPIXFMT  0x0C
#define ILI_RDIMGFMT  0x0D
#define ILI_RDSELFDIAG  0x0F

#define ILI_INVOFF  0x20
#define ILI_INVON   0x21
#define ILI_GAMMASET 0x26
#define ILI_DISPOFF 0x28
#define ILI_DISPON  0x29

#define ILI_CASET   0x2A
#define ILI_PASET   0x2B
#define ILI_RAMWR   0x2C
#define ILI_RAMRD   0x2E

#define ILI_PTLAR   0x30
#define ILI_MADCTL  0x36
#define ILI_PIXFMT  0x3A

#define ILI_FRMCTR1 0xB1
#define ILI_FRMCTR2 0xB2
#define ILI_FRMCTR3 0xB3
#define ILI_INVCTR  0xB4
#define ILI_DFUNCTR 0xB6

#define ILI_PWCTR1  0xC0
#define ILI_PWCTR2  0xC1
#define ILI_PWCTR3  0xC2
#define ILI_PWCTR4  0xC3
#define ILI_PWCTR5  0xC4
#define ILI_VMCTR1  0xC5
#define ILI_VMCTR2  0xC7

#define ILI_RDID1   0xDA
#define ILI_RDID2   0xDB
#define ILI_RDID3   0xDC
#define ILI_RDID4   0xDD

#define ILI_GMCTRP1 0xE0
#define ILI_GMCTRN1 0xE1
/*
#define ILI_PWCTR6  0xFC
*/


#define	ILI_R_POS_RGB   11	// Red last bit position for RGB display
#define	ILI_G_POS_RGB   5 	// Green last bit position for RGB display
#define	ILI_B_POS_RGB   0	// Blue last bit position for RGB display

#define	ILI_RGB(R,G,B) \
	(((uint16_t)(R >> 3) << ILI_R_POS_RGB) | \
	((uint16_t)(G >> 2) << ILI_G_POS_RGB) | \
	((uint16_t)(B >> 3) << ILI_B_POS_RGB))

#define	ILI_R_POS_BGR   0	// Red last bit position for BGR display
#define	ILI_G_POS_BGR   5 	// Green last bit position for BGR display
#define	ILI_B_POS_BGR   11	// Blue last bit position for BGR display

#define	ILI_BGR(R,G,B) \
	(((uint16_t)(R >> 3) << ILI_R_POS_BGR) | \
	((uint16_t)(G >> 2) << ILI_G_POS_BGR) | \
	((uint16_t)(B >> 3) << ILI_B_POS_BGR))


#define ILI_COLOR_BLACK       ILI_BGR(0,     0,   0)
#define ILI_COLOR_NAVY        ILI_BGR(0,     0, 123)
#define ILI_COLOR_DARKGREEN   ILI_BGR(0,   125,   0)
#define ILI_COLOR_DARKCYAN    ILI_BGR(0,   125, 123)
#define ILI_COLOR_MAROON      ILI_BGR(123,   0,   0)
#define ILI_COLOR_PURPLE      ILI_BGR(123,   0, 123)
#define ILI_COLOR_OLIVE       ILI_BGR(123, 125,   0)
#define ILI_COLOR_LIGHTGREY   ILI_BGR(198, 195, 198)
#define ILI_COLOR_DARKGREY    ILI_BGR(123, 125, 123)
#define ILI_COLOR_BLUE        ILI_BGR(0,     0, 255)
#define ILI_COLOR_GREEN       ILI_BGR(0,   255,   0)
#define ILI_COLOR_CYAN        ILI_BGR(0,   255, 255)
#define ILI_COLOR_RED         ILI_BGR(255,   0,   0)
#define ILI_COLOR_MAGENTA     ILI_BGR(255,   0, 255)
#define ILI_COLOR_YELLOW      ILI_BGR(255, 255,   0)
#define ILI_COLOR_WHITE       ILI_BGR(255, 255, 255)
#define ILI_COLOR_ORANGE      ILI_BGR(255, 165,   0)
#define ILI_COLOR_GREENYELLOW ILI_BGR(173, 255,  41)
#define ILI_COLOR_PINK        ILI_BGR(255, 130, 198)


/*************************** Pin confirugation START ************************/

#define ILI_PORT_DATA	GPIOB
#define ILI_D0			GPIO0
#define ILI_D1			GPIO1
#define ILI_D2			GPIO2
#define ILI_D3			GPIO3
#define ILI_D4			GPIO4
#define ILI_D5			GPIO5
#define ILI_D6			GPIO6
#define ILI_D7			GPIO7
#define ILI_PORT_CTRL_A	GPIOA
#define ILI_PORT_CTRL_B	GPIOB
#define ILI_RD			GPIO6
#define ILI_RST			GPIO11
#define ILI_PORT_CTRL_C	GPIOC
#define ILI_CS			GPIO13
#define ILI_DC			GPIO14
#define ILI_WR			GPIO15
#define JTAG_REMAPPING_MODE AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF /* See below */

/*
Possible values of JTAG_REMAPPING_MODE:
AFIO_MAPR_SWJ_CFG_FULL_SWJ: Full Serial Wire JTAG capability
AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_JNTRST: Full Serial Wire JTAG capability without JNTRST (PB4 as GPIO)
AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON: JTAG-DP disabled with SW-DP enabled
AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF: JTAG-DP disabled and SW-DP disabled
*/

/*
 * >>>>>> ATTENTION: if there are other ports in use, e.g. GPIOC, <<<<<<
 * >>>>>>   please, update CONFIG_GPIO_CLOCK() and CONFIG_GPIO()  <<<<<<
 * >>>>>>              macros below accordingly.                  <<<<<<
 */
/*************************** Pin confirugation END ************************/

#define ILI_RD_ACTIVE		GPIO_BRR(ILI_PORT_CTRL_A) = ILI_RD
#define ILI_RD_IDLE			GPIO_BSRR(ILI_PORT_CTRL_A) = ILI_RD
#define ILI_WR_ACTIVE		GPIO_BRR(ILI_PORT_CTRL_C) = ILI_WR
#define ILI_WR_IDLE			GPIO_BSRR(ILI_PORT_CTRL_C) = ILI_WR
#define ILI_DC_CMD			GPIO_BRR(ILI_PORT_CTRL_C) = ILI_DC
#define ILI_DC_DAT			GPIO_BSRR(ILI_PORT_CTRL_C) = ILI_DC
#define ILI_CS_ACTIVE		GPIO_BRR(ILI_PORT_CTRL_C) = ILI_CS
#define ILI_CS_IDLE			GPIO_BSRR(ILI_PORT_CTRL_C) = ILI_CS
#define ILI_RST_ACTIVE		GPIO_BRR(ILI_PORT_CTRL_B) = ILI_RST
#define ILI_RST_IDLE		GPIO_BSRR(ILI_PORT_CTRL_B) = ILI_RST

#define ILI_WR_STROBE		{ILI_WR_ACTIVE; ILI_WR_IDLE;}
#define ILI_RD_STROBE		{ILI_RD_ACTIVE; ILI_RD_IDLE;}

#define ILI_WRITE_8BIT(d)	{GPIO_BSRR(ILI_PORT_DATA) = (uint32_t)(0x00FF0000 | ((d) & 0xFF)); ILI_WR_STROBE;}
#define ILI_READ_8BIT(d)	{d = (uint8_t)(GPIO_IDR(ILI_PORT_DATA) & 0x00FF);}

	#define ILI_CONFIG_GPIO_CLOCK()	    { \
										rcc_periph_clock_enable(RCC_GPIOB); \
										rcc_periph_clock_enable(RCC_GPIOA); \
										rcc_periph_clock_enable(RCC_GPIOC); \
										rcc_periph_clock_enable(RCC_AFIO); \
									}
	#define ILI_CONFIG_GPIO()			{ \
										/*Configure ILI_PORT_DATA GPIO pins */ \
										gpio_set_mode( \
											ILI_PORT_DATA, \
											GPIO_MODE_OUTPUT_50_MHZ, \
											GPIO_CNF_OUTPUT_PUSHPULL, \
											ILI_D0 | ILI_D1 | ILI_D2 | ILI_D3 | ILI_D4 | ILI_D5 | ILI_D6 | ILI_D7); \
										/*Configure ILI_PORT_CTRL_B GPIO pins */ \
										gpio_set_mode(ILI_PORT_CTRL_B, \
											GPIO_MODE_OUTPUT_50_MHZ, \
											GPIO_CNF_OUTPUT_PUSHPULL, \
											ILI_RD | ILI_RST); \
										/*Configure ILI_PORT_CTRL_C GPIO pins */ \
										gpio_set_mode(ILI_PORT_CTRL_C, \
											GPIO_MODE_OUTPUT_50_MHZ, \
											GPIO_CNF_OUTPUT_PUSHPULL, \
											ILI_CS | ILI_DC | ILI_WR); \
										/*Configure GPIO pin Output Level */ \
										gpio_set( \
											ILI_PORT_DATA, \
											ILI_D0 | ILI_D1 | ILI_D2 | ILI_D3 | ILI_D4 | ILI_D5 | ILI_D6 | ILI_D7); \
										gpio_set( \
											ILI_PORT_CTRL_B, \
											ILI_RD | ILI_RST); \
										gpio_set( \
											ILI_PORT_CTRL_C, \
											ILI_CS | ILI_DC | ILI_WR); \
										/* Remap JTAG pins */ \
										/*AFIO_MAPR |= JTAG_REMAPPING_MODE;*/ \
									}


#define ILI_SWAP(a, b)		{uint16_t temp; temp = a; a = b; b = temp;}

/*
* function prototypes
*/

/*
 * Inline function to send 8 bit command to the display
 * User need not call it
 */
__attribute__((always_inline)) static inline void _ili_write_command_8bit(uint8_t cmd)
{
	//CS_ACTIVE;
	ILI_DC_CMD;
	ILI_WRITE_8BIT(cmd);
}

/*
 * Inline function to send 8 bit data to the display
 * User need not call it
 */
__attribute__((always_inline)) static inline  void _ili_write_data_8bit(uint8_t dat)
{
	//CS_ACTIVE;
	ILI_DC_DAT;
	ILI_WRITE_8BIT(dat);
}

/*
 * Inline function to send 16 bit data to the display
 * User need not call it
 */
__attribute__((always_inline)) static inline void _ili_write_data_16bit(uint16_t dat)
{
	//CS_ACTIVE;
	ILI_DC_DAT;
	ILI_WRITE_8BIT((uint8_t)(dat >> 8));
	ILI_WRITE_8BIT((uint8_t)dat);
}

/**
 * Set an area for drawing on the display with start row,col and end row,col.
 * User don't need to call it usually, call it only before some functions who don't call it by default.
 * @param x1 start column address.
 * @param y1 start row address.
 * @param x2 end column address.
 * @param y2 end row address.
 */
void ili_set_address_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * Fills `len` number of pixels with `color`.
 * Call ili_set_address_window() before calling this function.
 * @param color 16-bit RGB565 color value
 * @param len 32-bit number of pixels
 */
void ili_fill_color(uint16_t color, uint32_t len);

/**
 * Rotate the display clockwise or anti-clockwie set by `rotation`
 * @param rotation Type of rotation. Supported values 0, 1, 2, 3
 */
void ili_rotate_display(uint8_t rotation);

/**
 * Initialize the display driver
 */
void ili_init();

/**
 * Fills a rectangular area with `color`.
 * Before filling, performs area bound checking
 * @param x Start col address
 * @param y Start row address
 * @param w Width of rectangle
 * @param h Height of rectangle
 * @param color 16-bit RGB565 color
 */
void ili_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/*
 * Same as `ili_fill_rect()` but does not do bound checking, so it's slightly faster
 */
void ili_fill_rect_fast(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/**
 * Fill the entire display (screen) with `color`
 * @param color 16-bit RGB565 color
 */
void ili_fill_screen(uint16_t color);

/*
/**
 * Draw a pixel at a given position with `color`
 * @param x Start col address
 * @param y Start row address
 */
void ili_draw_pixel(uint16_t x, uint16_t y, uint16_t color);

void ili_dataBegin();
void ili_dataEnd();
void ili_sendWords(int nb, const uint16_t *data);

//------------------------------------------------------------------------
#endif /* INC_ILI9341_STM32_PARALLEL8_H_ */
