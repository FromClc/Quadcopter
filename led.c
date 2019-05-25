#include "led.h"


void led1_init(void){
	GPIO_InitTypeDef gt;
	RCC_APB2PeriphClockCmd(LED1_RCC,ENABLE);
	
	gt.GPIO_Pin=LED1_PIN;
	gt.GPIO_Mode=GPIO_Mode_Out_PP;
	gt.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LED1_GPIO,&gt);
	GPIO_ResetBits(LED1_GPIO, LED1_PIN);
	
}
void led_ct(uint8_t num){
	if(num==OPEN)

GPIO_SetBits(LED1_GPIO, LED1_PIN);
	
else if(num==CLOSE)
	GPIO_ResetBits(LED1_GPIO, LED1_PIN);
}
