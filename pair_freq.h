#ifndef _PAIR_FREQ_H
#define _PAIR_FREQ_H
#include "stm32f10x.h"

uint32_t  chip_id[3] = {0};  

uint32_t ID = 0;



typedef struct
{
    uint8_t flag ;
    uint8_t addr[5];
    uint8_t freq_channel[4];
}_PAIR;

void wait_pairing(void);
void get_chip_id(void);
extern _PAIR pair;





#endif
