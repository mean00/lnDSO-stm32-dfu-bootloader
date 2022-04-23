/*
 *  (C) 2021 MEAN00 fixounet@free.fr
 *  See license file
 */
#pragma once
void lnInitSystemClock();
enum Peripherals
    {
            pNONE=0,
            pGPIOA,
            pGPIOB,
            pGPIOC,
            pAF,
            pAPB1=100,
            pAPB2,
            pSYSCLOCK,
    };
// EOF