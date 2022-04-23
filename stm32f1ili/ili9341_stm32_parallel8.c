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
#include <stdlib.h>
#include <ili9341_stm32_parallel8.h>

//TFT width and height default global variables
uint16_t ili_tftwidth = 320;
uint16_t ili_tftheight = 240;



/**
 * Set an area for drawing on the display with start row,col and end row,col.
 * User don't need to call it usually, call it only before some functions who don't call it by default.
 * @param x1 start column address.
 * @param y1 start row address.
 * @param x2 end column address.
 * @param y2 end row address.
 */
void ili_set_address_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	_ili_write_command_8bit(ILI_CASET);

	ILI_DC_DAT;
	ILI_WRITE_8BIT((uint8_t)(x1 >> 8));
	ILI_WRITE_8BIT((uint8_t)x1);
	ILI_WRITE_8BIT((uint8_t)(x2 >> 8));
	ILI_WRITE_8BIT((uint8_t)x2);


	_ili_write_command_8bit(ILI_PASET);
	ILI_DC_DAT;
	ILI_WRITE_8BIT((uint8_t)(y1 >> 8));
	ILI_WRITE_8BIT((uint8_t)y1);
	ILI_WRITE_8BIT((uint8_t)(y2 >> 8));
	ILI_WRITE_8BIT((uint8_t)y2);

	_ili_write_command_8bit(ILI_RAMWR);
}



/**
 * Fills `len` number of pixels with `color`.
 * Call ili_set_address_window() before calling this function.
 * @param color 16-bit RGB565 color value
 * @param len 32-bit number of pixels
 */
void ili_fill_color(uint16_t color, uint32_t len)
{
	/*
	* Here, macros are directly called (instead of inline functions) for performance increase
	*/
	uint16_t blocks = (uint16_t)(len / 64); // 64 pixels/block
	uint8_t  pass_count;
	uint8_t color_high = color >> 8;
	uint8_t color_low = color;

	ILI_DC_DAT;
	// Write first pixel
	ILI_WRITE_8BIT(color_high); ILI_WRITE_8BIT(color_low);
	len--;

	// If higher byte and lower byte are identical,
	// just strobe the WR pin to send the previous data
	if(color_high == color_low)
	{
		while(blocks--)
		{
			// pass count = number of blocks / pixels per pass = 64 / 4
			pass_count = 16;
			while(pass_count--)
			{
				ILI_WR_STROBE; ILI_WR_STROBE; ILI_WR_STROBE; ILI_WR_STROBE; // 2
				ILI_WR_STROBE; ILI_WR_STROBE; ILI_WR_STROBE; ILI_WR_STROBE; // 4
			}
		}
		// Fill any remaining pixels (1 to 64)
		pass_count = len & 63;
		while (pass_count--)
		{
			ILI_WR_STROBE; ILI_WR_STROBE;
		}
	}

	// If higher and lower bytes are different, send those bytes
	else
	{
		while(blocks--)
		{
			pass_count = 16;
			while(pass_count--)
			{
				ILI_WRITE_8BIT(color_high); ILI_WRITE_8BIT(color_low); 	ILI_WRITE_8BIT(color_high); ILI_WRITE_8BIT(color_low); //2
				ILI_WRITE_8BIT(color_high); ILI_WRITE_8BIT(color_low); 	ILI_WRITE_8BIT(color_high); ILI_WRITE_8BIT(color_low); //4
			}
		}
		pass_count = len & 63;
		while (pass_count--)
		{
			// write here the remaining data
			ILI_WRITE_8BIT(color_high); ILI_WRITE_8BIT(color_low);
		}
	}
}

/**
 * Fill the entire display (screen) with `color`
 * @param color 16-bit RGB565 color
 */
void ili_fill_screen(uint16_t color)
{
	ili_set_address_window(0, 0, ili_tftwidth - 1, ili_tftheight - 1);
	ili_fill_color(color, (uint32_t)ili_tftwidth * (uint32_t)ili_tftheight);
}


/**
 * Draw a pixel at a given position with `color`
 * @param x Start col address
 * @param y Start row address
 */
void ili_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
	/*
	* Why?: This function is mainly added in the driver so that  ui libraries can use it.
	* example: LittlevGL requires user to supply a function that can draw pixel
	*/

	ili_set_address_window(x, y, x, y);
	ILI_DC_DAT;
	ILI_WRITE_8BIT((uint8_t)(color >> 8));
	ILI_WRITE_8BIT((uint8_t)color);
}



/**
 * Rotate the display clockwise or anti-clockwie set by `rotation`
 * @param rotation Type of rotation. Supported values 0, 1, 2, 3
 */
void ili_rotate_display(uint8_t rotation)
{
	/*
	* 	(uint8_t)rotation :	Rotation Type
	* 					0 : Default landscape
	* 					1 : Potrait 1
	* 					2 : Landscape 2
	* 					3 : Potrait 2
	*/

#ifdef USER_DEFAULT_PLATFORM
    uint16_t new_height = 240;
    uint16_t new_width = 320;
#elif DSO138_PLATFORM
    uint16_t new_height = 320;
    uint16_t new_width = 240;
#endif

	switch (rotation)
	{
		case 0:
			_ili_write_command_8bit(ILI_MADCTL);		//Memory Access Control
			_ili_write_data_8bit(0x40);				//MX: 1, MY: 0, MV: 0	(Landscape 1. Default)
			ili_tftheight = new_height;
			ili_tftwidth = new_width;
			break;
		case 1:
			_ili_write_command_8bit(ILI_MADCTL);		//Memory Access Control
			_ili_write_data_8bit(0x20);				//MX: 0, MY: 0, MV: 1	(Potrait 1)
			ili_tftheight = new_width;
			ili_tftwidth = new_height;
			break;
		case 2:
			_ili_write_command_8bit(ILI_MADCTL);		//Memory Access Control
			_ili_write_data_8bit(0x80);				//MX: 0, MY: 1, MV: 0	(Landscape 2)
			ili_tftheight = new_height;
			ili_tftwidth = new_width;
			break;
		case 3:
			_ili_write_command_8bit(ILI_MADCTL);		//Memory Access Control
			_ili_write_data_8bit(0xE0);				//MX: 1, MY: 1, MV: 1	(Potrait 2)
			ili_tftheight = new_width;
			ili_tftwidth = new_height;
			break;
	}
}

/**
 * Initialize the display driver
 */
void ili_init()
{
	// Set gpio clock
	// MEANX ILI_CONFIG_GPIO_CLOCK();
	// Configure gpio output dir and mode
	ILI_CONFIG_GPIO();

	ILI_CS_ACTIVE;

	ILI_RST_IDLE;
	ILI_RST_ACTIVE;
	ILI_RST_IDLE;

	// Approx 10ms delay at 128MHz clock
	for (uint32_t i = 0; i < 2000000; i++)
		__asm__("nop");

	_ili_write_command_8bit(0xEF);
	_ili_write_data_8bit(0x03);
	_ili_write_data_8bit(0x80);
	_ili_write_data_8bit(0x02);

	_ili_write_command_8bit(0xCF);
	_ili_write_data_8bit(0x00);
	_ili_write_data_8bit(0XC1);
	_ili_write_data_8bit(0X30);

	_ili_write_command_8bit(0xED);
	_ili_write_data_8bit(0x64);
	_ili_write_data_8bit(0x03);
	_ili_write_data_8bit(0X12);
	_ili_write_data_8bit(0X81);

	_ili_write_command_8bit(0xE8);
	_ili_write_data_8bit(0x85);
	_ili_write_data_8bit(0x00);
	_ili_write_data_8bit(0x78);

	_ili_write_command_8bit(0xCB);
	_ili_write_data_8bit(0x39);
	_ili_write_data_8bit(0x2C);
	_ili_write_data_8bit(0x00);
	_ili_write_data_8bit(0x34);
	_ili_write_data_8bit(0x02);

	_ili_write_command_8bit(0xF7);
	_ili_write_data_8bit(0x20);

	_ili_write_command_8bit(0xEA);
	_ili_write_data_8bit(0x00);
	_ili_write_data_8bit(0x00);

	_ili_write_command_8bit(ILI_PWCTR1);    //Power control
	_ili_write_data_8bit(0x23);   //VRH[5:0]

	_ili_write_command_8bit(ILI_PWCTR2);    //Power control
	_ili_write_data_8bit(0x10);   //SAP[2:0];BT[3:0]

	_ili_write_command_8bit(ILI_VMCTR1);    //VCM control
	_ili_write_data_8bit(0x3e);
	_ili_write_data_8bit(0x28);

	_ili_write_command_8bit(ILI_VMCTR2);    //VCM control2
	_ili_write_data_8bit(0x86);  //--

	_ili_write_command_8bit(ILI_MADCTL);    // Memory Access Control
	_ili_write_data_8bit(0x40); // Rotation 0 (landscape mode)

	_ili_write_command_8bit(ILI_PIXFMT);
	_ili_write_data_8bit(0x55);

	_ili_write_command_8bit(ILI_FRMCTR1);
	_ili_write_data_8bit(0x00);
	_ili_write_data_8bit(0x13); // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz

	_ili_write_command_8bit(ILI_DFUNCTR);    // Display Function Control
	_ili_write_data_8bit(0x08);
	_ili_write_data_8bit(0x82);
	_ili_write_data_8bit(0x27);

	_ili_write_command_8bit(0xF2);    // 3Gamma Function Disable
	_ili_write_data_8bit(0x00);

	_ili_write_command_8bit(ILI_GAMMASET);    //Gamma curve selected
	_ili_write_data_8bit(0x01);

	_ili_write_command_8bit(ILI_GMCTRP1);    //Set Gamma
	_ili_write_data_8bit(0x0F);
	_ili_write_data_8bit(0x31);
	_ili_write_data_8bit(0x2B);
	_ili_write_data_8bit(0x0C);
	_ili_write_data_8bit(0x0E);
	_ili_write_data_8bit(0x08);
	_ili_write_data_8bit(0x4E);
	_ili_write_data_8bit(0xF1);
	_ili_write_data_8bit(0x37);
	_ili_write_data_8bit(0x07);
	_ili_write_data_8bit(0x10);
	_ili_write_data_8bit(0x03);
	_ili_write_data_8bit(0x0E);
	_ili_write_data_8bit(0x09);
	_ili_write_data_8bit(0x00);

	_ili_write_command_8bit(ILI_GMCTRN1);    //Set Gamma
	_ili_write_data_8bit(0x00);
	_ili_write_data_8bit(0x0E);
	_ili_write_data_8bit(0x14);
	_ili_write_data_8bit(0x03);
	_ili_write_data_8bit(0x11);
	_ili_write_data_8bit(0x07);
	_ili_write_data_8bit(0x31);
	_ili_write_data_8bit(0xC1);
	_ili_write_data_8bit(0x48);
	_ili_write_data_8bit(0x08);
	_ili_write_data_8bit(0x0F);
	_ili_write_data_8bit(0x0C);
	_ili_write_data_8bit(0x31);
	_ili_write_data_8bit(0x36);
	_ili_write_data_8bit(0x0F);

	_ili_write_command_8bit(ILI_SLPOUT);    //Exit Sleep
	//delay 150ms if display output is inaccurate

	_ili_write_command_8bit(ILI_DISPON);    //Display on
	//delay 150ms if display output is inaccurate
}
