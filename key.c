#include "key.h"
#include "sys.h"
#include "delay.h"


void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
        
//    RCC_APB2PeriphClockCmd(key_left_rcc, ENABLE);  
    RCC_APB2PeriphClockCmd(key_right_rcc, ENABLE);
    
//    GPIO_InitStructure.GPIO_Pin = key_left_pin;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;          
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(key_left_port, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = key_right_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;          
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(key_right_port, &GPIO_InitStructure); 
}

//����ɨ��
//0����֧��������
//1��֧��������
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;
    
    if(mode)  
        key_up = 1;
    if(key_up&&(key_right==0))
    {
        delay_ms(5);
        key_up = 0;
        if(key_right==0)    return 2;
    }
    else if(key_right==1)
    {
        key_up = 1;
    }
    return 0;
}
