#pragma once

extern "C"
{
    #include "heatshrink_decoder.h"
}
/**
 */
class iliHS
{
public:
    iliHS(const uint8_t *data)
    {
        heatshrink_decoder_reset(&hsd); 
         head=tail=decompBuffer;
         p=data;
    }
    ~iliHS()
    {
    }
    uint8_t next()
    {
        size_t count;
        if(head==tail)
        { 
            again:
           // Data available ?
           int r=heatshrink_decoder_poll(&hsd, decompBuffer,64,&count);
           if(r==HSDR_POLL_EMPTY && count==0)
           {

                int r=heatshrink_decoder_sink(&hsd, (uint8_t *)p, 64*1024, &count) ;
                xAssert(r>=0);
                p+=count;
                goto again;
           }
           head=decompBuffer;
           tail=head+count;

        } // nest
        return *head++;
    }
protected:
    heatshrink_decoder hsd ;
    uint8_t decompBuffer[64];
    uint8_t *tail;
    uint8_t *head;   
    const uint8_t *p;
};