#ifndef _LED_H
#define _LED_H
#include "stm32f10x.h"

#define LED1_GPIO   GPIOA
#define LED1_RCC     RCC_APB2Periph_GPIOB
#define LED1_PIN      GPIO_Pin_10
#define CLOSE 0
#define OPEN 1

void led1_init(void);
void led_ct(uint8_t num);


#endif 
