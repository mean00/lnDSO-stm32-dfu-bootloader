/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 
  This is a 8 bit parrallel mode for the DSO150
  The code is a bit hardcoded for the following pinout, you can easily duplicate/change it 
  for another 8 bit parallel screen
 * 
 * Read Signal PA6
 * Write Signal PC15
 * ChipSelect  PC13
 * Reset       PB9
 * RS/CD       PC14
 * 
 * DATA = PB0...PB7
 
 */
#pragma once
#include "simpler9341.h"

// There is some issues here
// If the speed is above 96Mhz, the GD32f303 will white screen after a while
// it is either the gpio speed (max 50 Mhz) or some LCD timing issue
// adding a nop is enough to fix the issue
// with the STM32 it should be fine due to the flash wait state

// with the GD32VF103 it still white screens after a bit, even with a low speed


#if LN_ARCH == LN_ARCH_RISCV
     #if LN_MCU_SPEED > 96000000
            #define ILI_NOP __asm("nop");
        #else
            #define ILI_NOP {}
     #endif
#else
    #if LN_ARCH == LN_ARCH_ARM
        #if LN_MCU_SPEED > 96000000
            #define ILI_NOP __asm("nop");
        #else
            #define ILI_NOP {}
        #endif
    #else
        #error UNSUPPORTED ARCH
    #endif
#endif
//
class noplnFastIO : public  lnFastIO
{
public:
        noplnFastIO(lnPin p) : lnFastIO(p)
        {
            
        }
    void pulseLowNop() __attribute__((always_inline)) { *_onoff=_offbit;ILI_NOP;*_onoff=_onbit;ILI_NOP;}
};

/**
 * 
 * @param bop
 * @param writePin
 */
class lnFast8bitIo: public lnFastIO
{
    public : 
                lnFast8bitIo( uint32_t *bop, lnPin writePin) : lnFastIO(writePin)
                {
                    _bop=bop;
                }
                void pulseLowNop() { *_onoff=_offbit;ILI_NOP;*_onoff=_onbit;ILI_NOP}
                void pulsesLowNop(int count);
                void pulseData(int count, int hi, int lo);                
                void sendBlock(int nb, uint16_t *data);
                
    protected:                
                volatile uint32_t *_bop;
};

/**
 * 
 * @param w
 * @param h
 * @param spi
 * @param pinDc
 * @param pinCS
 */
class ln8bit9341 : public ili9341
{
    public:
                        ln8bit9341(int w, int h , int port,  int pinDc, int pinCS, int pinWrite, int pinRead, int pinReset);
            virtual      ~ln8bit9341();
            virtual void init(const uint8_t *init1, const uint8_t *init2);
            virtual void sendByte(int byte); // 8 bytes
            virtual void sendWord(int byte); // 16 bytes
            virtual void sendBytes(int nb, const uint8_t *data); // 8 bits
            virtual void sendWords(int nb, const uint16_t *data); // 16 bits
            virtual void floodWords(int nb, const uint16_t data); // 16 bits
            virtual void updateHwRotation();
            virtual void setAddress(int x, int y, int w, int h);            
            virtual void dataBegin();
            virtual void dataEnd();
            virtual void pushColors(int len, uint16_t *data);
            
            
                    uint32_t readChipId();
    protected:
                    void reset();
                    void writeCmdParam(uint16_t cmd, int payload, const uint8_t * data);
                    void writeCommand(uint16_t c);
                    void sendSequence(const uint8_t *data);
                    void write8(uint8_t c);
                    uint8_t read8();
                    uint32_t readRegister32(int reg);
                    void writeRegister32(int r,uint32_t  val);
                    void setReadDir();
                    void setWriteDir();
                    
            noplnFastIO  _ioRead, _ioCS, _ioDC;
            lnFast8bitIo *_ioWrite;
            lnPin       _pinReset;
            int         _dataPort;
            uint32_t    *_bop;
};
// EOF
