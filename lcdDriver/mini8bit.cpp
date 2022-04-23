/*
 *  (C) Adafruit
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 *
 * This is a 8 bit parrallel mode for the ILI9341/ST7789 as present on the DSO150
 * With the GD32vF103 there is a timing issue so it is slowed down a bit
 * Not sure if the timing issue is due to the speed (running at 108 Mhz with max
 * GPIO speed = 50 Mhz + speed of ILI ignals) or if it is due to fence issue
 *
 *
 *
 */
#include "mini8bit.h"
#include "simpler9341_priv.h"

#define LOW_LEVEL_PRINT(...)                                                                                           \
    {                                                                                                                  \
    }
#define WR_DATA8(cc) ((((cc ^ 0xFF) << 16) | (cc)))
#define IS_7789() (_chipId == 0x7789)
#define CHECK_ARBITER()                                                                                                \
    {                                                                                                                  \
    }

/**
 *
 * @param w
 * @param h
 * @param spi
 * @param pinDc
 * @param pinCS
 */
ln8bit9341::ln8bit9341(int w, int h, int port, int pinDC, int pinCS, int pinWrite, int pinRead, int pinReset)
    : ili9341(w, h), _ioRead(pinRead), _ioCS(pinCS), _ioDC(pinDC)
{
    _dataPort = port;
    _pinReset = pinReset;
    _PhysicalXoffset = 0;
    _PhysicalYoffset = 0;
    _bop = (uint32_t *)lnGetGpioToggleRegister(_dataPort);
    _ioWrite = new lnFast8bitIo(_bop, pinWrite);
// pinCS = PC13
// pinDC = PC14
// pinWrite = PC15
#if 0 // WORKS!
    bool onoff=true;       
    while(1)
    {
     //   xDelay(5);
        _ioCS.pulseLowNop();
        _ioWrite->pulseLowNop();
        _ioDC.pulseLowNop();
        _ioRead.pulseLowNop();
    }
#endif

#if 0 // WORKS!
    bool onoff=true;
    lnDigitalWrite(PC13,true);
    lnDigitalWrite(PC14,false);
    lnDigitalWrite(PC15,true);
    
    while(1)
    {
        xDelay(500);
        lnDigitalWrite(PC13,0);
        lnDigitalWrite(PC14,1);
        lnDigitalWrite(PC15,0);
        xDelay(500);
        lnDigitalWrite(PC13,1);
        lnDigitalWrite(PC14,0);
        lnDigitalWrite(PC15,1);


        onoff=!onoff;
        Logger("*Hey*\n");
    }
#endif

#if 0

    lnPinMode(PINPIN,lnOUTPUT);
    while(1)
    {
                
        xDelay(1);
        lnDigitalWrite(PINPIN,1);
        //_ioCS.pulseLow(); // 13
        //_ioWrite->pulseLow(); //15 
        //_ioDC.pulseLow(); //14 
        xDelay(1);
        lnDigitalWrite(PINPIN,0);
        
    }
#endif    

}
/**
 *
 */
ln8bit9341::~ln8bit9341()
{
}

#define CS_ACTIVE                                                                                                      \
    {                                                                                                                  \
        _ioCS.off();                                                                                                   \
    }
#define CS_IDLE                                                                                                        \
    {                                                                                                                  \
        _ioCS.on();                                                                                                    \
    }

#define CD_DATA                                                                                                        \
    {                                                                                                                  \
        _ioDC.on();                                                                                                    \
    }
#define CD_COMMAND                                                                                                     \
    {                                                                                                                  \
        _ioDC.off();                                                                                                   \
    }

#define WR_ACTIVE                                                                                                      \
    {                                                                                                                  \
        _ioWrite->off();                                                                                               \
    }
#define WR_IDLE                                                                                                        \
    {                                                                                                                  \
        _ioWrite->on();                                                                                                \
    }

#define RD_ACTIVE                                                                                                      \
    {                                                                                                                  \
        _ioRead.off();                                                                                                 \
    }
#define RD_IDLE                                                                                                        \
    {                                                                                                                  \
        _ioRead.on();                                                                                                  \
    }

#define WR_STROBE                                                                                                      \
    {                                                                                                                  \
        _ioWrite->pulseLowNop();                                                                                       \
    }
#define CS_ACTIVE_CD_COMMAND                                                                                           \
    {                                                                                                                  \
        CS_ACTIVE;                                                                                                     \
        CD_COMMAND                                                                                                     \
    }

/*****************************************************************************/

void ln8bit9341::write8(uint8_t c)
{
    int cc = WR_DATA8((int)c);
    *_bop = cc;
    ILI_NOP;
    WR_STROBE;
}
/**
 * This is not used much at all. Only a few times at the beginning
 * No need to optimize
 * @return
 */
uint8_t ln8bit9341::read8()
{
    RD_ACTIVE;
    lnDelayUs(10);
    uint8_t temp = lnReadPort(_dataPort) & 0xff;
    lnDelayUs(10);
    RD_IDLE;
    lnDelayUs(10);
    return temp;
}

/**
 *
 * @param cmd
 * @param payload
 * @param data
 */
void ln8bit9341::writeCmdParam(uint16_t cmd, int payload, const uint8_t *data)
{
    CD_COMMAND;
    sendWord(cmd);
    if (payload)
    {
        CD_DATA;
        const uint8_t *tail = data + payload;
        while (data < tail)
        {
            write8(*(data++));
        }
    }
}
/**
 *
 * @param c
 */
void ln8bit9341::writeCommand(uint16_t c)
{
    writeCmdParam(c, 0, NULL);
}
//------------------------------
// This is *very* seldom used....
//-------------------------------
void ln8bit9341::setReadDir()
{
    for (int pin = PB0; pin <= PB7; pin++)
        lnPinMode(pin, lnINPUT_PULLDOWN);
}

void ln8bit9341::setWriteDir()
{
    for (int pin = PB0; pin <= PB7; pin++)
        lnPinMode(pin, lnOUTPUT);
}

/**
 * This is not used much
 * @param reg
 * @return
 */
uint32_t ln8bit9341::readRegister32(int r)
{
    uint32_t val;
    uint8_t x;

    CS_ACTIVE;
    setWriteDir();
    // try reading register #4
    writeCommand(r);
    setReadDir(); // Set up LCD data port(s) for READ operations
    CD_DATA;
    lnDelayUs(50);
    val = read8();
    val <<= 8;
    val |= read8();
    val <<= 8;
    val |= read8();
    ;
    val <<= 8;
    val |= read8();
    ;
    CS_IDLE;
    setWriteDir();
    return val;
}

/**
 *
 * @param reg
 * @return
 */
void ln8bit9341::writeRegister32(int r, uint32_t val)
{
    uint8_t flat[4] = {(uint8_t)(val >> 24), (uint8_t)((val >> 16) & 0xff), (uint8_t)((val >> 8) & 0xff),
                       (uint8_t)(val & 0xff)};
    writeCmdParam(r, 4, flat);
}

/**
 *
 */
uint32_t ln8bit9341::readChipId()
{
    uint32_t regD3 = readRegister32(0xd3) & 0xffff;
    uint32_t reg04 = readRegister32(0x04) & 0xffff;

    if (regD3 == 0x9341)
        return 0x9341; // 9341
    if (reg04 == 0x8552)
        return 0x7789; // is it really a 7789 ?
    return 0;          // unknown
}

/**
 *
 */
void ln8bit9341::reset()
{
    _ioWrite->on();
    _ioRead.on();
    _ioCS.on();
    _ioDC.on();

    CS_IDLE;
    WR_IDLE;
    RD_IDLE;
    CD_DATA;
    setWriteDir();

    if (_pinReset != -1)
    {
        lnPinMode(_pinReset, lnOUTPUT);
        lnDigitalWrite(_pinReset, HIGH);
        xDelay(50);
        lnDigitalWrite(_pinReset, LOW);
        xDelay(50);
        lnDigitalWrite(_pinReset, HIGH);
        xDelay(50);
    }
    _chipId = readChipId();   
}
/**
 *
 * @param size
 * @param data
 */
void ln8bit9341::sendSequence(const uint8_t *data)
{
    CS_ACTIVE;
    while (*data)
    {
        uint8_t cmd = data[0];
        uint8_t len = data[1];
        data += 2;
        if (cmd == FAKE_DELAY_COMMAND)
        {
            delay(len);
            continue;
        }
        writeCmdParam(cmd, len, data);
        data += len;
    }
    CS_IDLE;
}

/**
 *
 */
void ln8bit9341::init(const uint8_t *init1, const uint8_t *init2)
{
    setWriteDir();
    reset();
    baseInit();

    sendSequence(init1);
    sendSequence(init2);
}
/**
 *
 * @param byte
 */
void ln8bit9341::sendByte(int c)
{
    write8(c);
}
/**
 *
 * @param byte
 */
void ln8bit9341::sendWord(int xbyte)
{
    int cc = (int)xbyte;
    int cl = cc & 0xff;
    int ch = cc >> 8;
    *_bop = WR_DATA8(ch);
    WR_STROBE;
    *_bop = WR_DATA8(cl);
    WR_STROBE;
}
/**
 *
 * @param nb
 * @param data
 */
void ln8bit9341::sendBytes(int nb, const uint8_t *data)
{
    const uint8_t *tail = data + nb;
    while (data < tail)
    {
        int cc = (int)*data++;
        *_bop = WR_DATA8(cc);
        WR_STROBE;
    }
}
/**
 *
 * @param nb
 * @param data
 */
void ln8bit9341::sendWords(int nb, const uint16_t *data)
{
    const uint16_t *tail = data + nb;
    while (data < tail)
    {
        int cc = (int)*data++;
        int cl = WR_DATA8(cc & 0xff);
        int ch = WR_DATA8(cc >> 8);
        *_bop = (ch);
        WR_STROBE;
        *_bop = (cl);
        WR_STROBE;
    }
}

/**
 *
 */
void ln8bit9341::dataBegin()
{
    CS_ACTIVE;
    CD_COMMAND;
    sendWord(ILI9341_MEMORYWRITE);
    CD_DATA;
}
/**
 *
 */
void ln8bit9341::dataEnd()
{
    CD_COMMAND;
    CS_IDLE;
}
/**
 *
 * @param nb
 * @param data
 *
 * GD32M4 : Simple = 33 ms, same = 28 ms
 * Quad16            26 ms  same = 21 ms
 *
 * pulsesLow Simple  = 26 ms , same = 8 ms
 *
 */
void ln8bit9341::floodWords(int nb, const uint16_t data)
{
    CHECK_ARBITER();

    uint16_t f = colorMap(data);
    register int cl = f & 0xff;
    register int ch = f >> 8;

    CS_ACTIVE;
    CD_COMMAND;
    sendWord(ILI9341_MEMORYWRITE);
    CD_DATA;
    if (1 && (f & 0xff) == (f >> 8)) // this does not work ?
    {
        uint32_t rpt = WR_DATA8(cl);
        *_bop = (rpt);
        _ioWrite->pulsesLowNop(nb);
        CS_IDLE;
        return;
    }
    _ioWrite->pulseData(nb, ch, cl);
    CS_IDLE;
}
/**
 */
static const uint8_t rotMode[4] = {0x8, 0xc8, 0x78, 0xa8};
/**
 *
 */
void ln8bit9341::updateHwRotation(void)
{
    uint8_t t;
    switch (_chipId)
    {
        case 0x9341:
            switch (_rotation)
            {
                case 1: t = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV; break;
                case 2: t = ILI9341_MADCTL_MX; break;
                case 3: t = ILI9341_MADCTL_MV; break;
                case 0:
                default: t = ILI9341_MADCTL_MY; break;
            }
            break;
        case 0x7789:
            switch (_rotation)
            {
                case 1: t = ILI9341_MADCTL_MY | ILI9341_MADCTL_MV; break;
                case 2: t = 0; break;
                case 3: t = ILI9341_MADCTL_MX | ILI9341_MADCTL_MV; break;
                case 0:
                default: t = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY; break;
            }
            break;
        default: xAssert(0); break;
    }
    t |= ILI9341_MADCTL_RGB;
    CS_ACTIVE;
    writeCmdParam(ILI9341_MADCTL, 1, &t);
    CS_IDLE;
}

/**
 *
 * @param x
 * @param y
 * @param w
 * @param h
 */
void ln8bit9341::setAddress(int x, int y, int w, int h)
{
    int a1, a2, b1, b2;
    a1 = x + _xOffset;
    a2 = a1 + w - 1;
    b1 = y + _yOffset;
    b2 = b1 + h - 1;
    CS_ACTIVE;
    writeRegister32(ILI9341_COLADDRSET, ((uint32_t)(a1 << 16) | a2));  // HX8357D uses same registers!
    writeRegister32(ILI9341_PAGEADDRSET, ((uint32_t)(b1 << 16) | b2)); // HX8357D uses same registers!
    CS_IDLE;
}
/**
 *
 * @param len
 * @param data
 */
void ln8bit9341::pushColors(int len, uint16_t *data)
{
    CHECK_ARBITER();
    dataBegin();
    _ioWrite->sendBlock(len, data);
    dataEnd();
}

//--- the time critical part follows
//--- If you use another implementation, this where the speed is lost

/**
 * 28 ms => 13 ms
 * \brief This is a high speed data sending of the same 16 byte pattern
 * @param count
 * @param hi
 * @param lo
 */
void lnFast8bitIo::sendBlock(int nb, uint16_t *data)
{

#define WRP                                                                                                            \
    *onoff = down;                                                                                                     \
    ILI_NOP;                                                                                                           \
    *onoff = up;                                                                                                       \
    ILI_NOP;

    register uint32_t up = _onbit, down = _offbit;
    volatile register uint32_t *bop = _bop, *onoff = _onoff;

    for (int i = 0; i < nb; i++)
    {
        int val = *data;
        int d = *(data++);
        int hi = WR_DATA8(d >> 8);
        int lo = WR_DATA8(d & 0xff);
        *bop = hi;
        WRP;
        *bop = lo;
        WRP;
    }
}
/**
 * 28 ms => 13 ms
 * \brief This is a high speed data sending of the same 16 byte pattern
 * @param count
 * @param hi
 * @param lo
 */
void lnFast8bitIo::pulseData(int nb, int hi, int lo)
{
#define SINGD                                                                                                          \
    *bop = (hh);                                                                                                       \
    WRP;                                                                                                               \
    *bop = (ll);                                                                                                       \
    WRP;
#define QUADD                                                                                                          \
    SINGD;                                                                                                             \
    SINGD;                                                                                                             \
    SINGD;                                                                                                             \
    SINGD

#if 1
    register uint32_t hh = WR_DATA8(hi), ll = WR_DATA8(lo), up = _onbit, down = _offbit;
    volatile register uint32_t *bop = _bop, *onoff = _onoff;

    for (int i = 0; i < nb; i++)
    {
        SINGD
    }
#else

    register uint32_t hh = WR_DATA8(hi), ll = WR_DATA8(lo), up = _onbit, down = _offbit;
    volatile register uint32_t *bop = _bop, *onoff = _onoff;

    int block = nb >> 4;
    int leftover = nb & 15;
    for (int i = 0; i < block; i++)
    {
        QUADD;
        QUADD;
        QUADD;
        QUADD;
    }
    for (int i = 0; i < leftover; i++)
    {
        SINGD
    }
#endif
}
/**
 *
 * @param count
 */
void lnFast8bitIo::pulsesLowNop(int nb)
{
#define PULSE                                                                                                          \
    WRP;                                                                                                               \
    WRP;
#undef QUADD
#define QUADD                                                                                                          \
    PULSE;                                                                                                             \
    PULSE;                                                                                                             \
    PULSE;                                                                                                             \
    PULSE

#if 1
    register uint32_t up = _onbit, down = _offbit;
    volatile register uint32_t *onoff = _onoff;
    for (int i = 0; i < nb*2; i++)
    {
        WRP
    }
#else
    register uint32_t up = _onbit, down = _offbit;
    volatile register uint32_t *onoff = _onoff;

    int block = nb >> 4;
    int leftover = nb & 15;
    for (int i = 0; i < block; i++)
    {
        QUADD;
        QUADD;
        QUADD;
        QUADD;
    }
    for (int i = 0; i < leftover; i++)
    {
        PULSE
    }
#endif
}

// EOF
