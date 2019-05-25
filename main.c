#include "stm32f10x.h"
#include "nrf24l01.h"
#include "nvic.h"
#include "uart.h"
#include "led.h"
#include "sys.h"
#include "delay.h"

 void Delay(u32 count)
 {
  u32 i=0;
  for(;i<count;i++);
 }
 
 int main(void)
 {	
	 uint8_t rx_buf[32];
	 delay_init();
	 //Usart1_Init(115200);
	 NVIC_config();
	 spi1_init();
	 led1_init();

	  while(NRF24L01_Check()){
			led_ct(OPEN);
      delay_ms(500);
			led_ct(CLOSE);
			delay_ms(500);
		}
		
	led_ct(CLOSE);
	 NRF_Init_Mode(40,RX);
	 while(1){
		if(NRF24L01_RxPacket(rx_buf)==0){
			
			if(rx_buf[31]==1&&rx_buf[0]==1)//接收到按键按下即灯常亮
			{
				led_ct(OPEN);
				delay_ms(3000);
				break;
			}
			else
			{
				led_ct(OPEN);
			delay_ms(500);
				led_ct(CLOSE);
			 delay_ms(500);
			} 
		}
			
	 }
	 while(1);
 }

