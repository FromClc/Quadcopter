#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"


//typedef struct
//{
//    uint8_t value;
//    uint8_t l;
//    uint8_t r;
//    uint16_t l_num;
//    uint16_t r_num;
//}_KEY;


//时钟宏定义区
//#define key_left_rcc     RCC_APB2Periph_GPIOC   
#define key_right_rcc    RCC_APB2Periph_GPIOA  
//端口宏定义区
//#define key_left_port    GPIOC
#define key_right_port   GPIOB
//引脚宏定义区
//#define key_left_pin     GPIO_Pin_14
#define key_right_pin    GPIO_Pin_11

//#define key_left   GPIO_ReadInputDataBit(key_left_port,key_left_pin)
#define key_right  GPIO_ReadInputDataBit(key_right_port,key_right_pin)


//extern _KEY key;

void key_init(void);
uint8_t key_scan(uint8_t mode);


#endif
