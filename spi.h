#ifndef _SPI_H
#define _SPI_H

#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "nrf24l01.h"
#include "uart.h"

#define RCC_SPI1_CLK			 RCC_APB2Periph_SPI1
#define SPI1_GPIO_PORT     GPIOA
#define RCC_SPI1_PORT      RCC_APB2Periph_GPIOA
#define SPI1_SCK           GPIO_Pin_5
#define SPI1_MISO          GPIO_Pin_6
#define SPI1_MOSI          GPIO_Pin_7
#define SPI1_SCK_SOURCE             GPIO_PinSource5
#define SPI1_MISO_SOURCE            GPIO_PinSource6
#define SPI1_MOSI_SOURCE            GPIO_PinSource7


//NRF24L01控制操作
#define NRF24L01_CE       GPIO_Pin_0
#define GPIO_NRF24L01_CE  GPIOB
#define RCC_NRF24L01_CE   RCC_APB2Periph_GPIOB

//NRF24L01 SPI接口CSN信号
#define NRF24L01_CSN       GPIO_Pin_4
#define GPIO_NRF24L01_CSN  GPIOA
#define RCC_NRF24L01_CSN   RCC_APB2Periph_GPIOA

#define NRF24L01_IRQ       GPIO_Pin_1	
#define GPIO_NRF24L01_IRQ  GPIOB
#define RCC_NRF24L01_IRQ   RCC_APB2Periph_GPIOB

// SPI总线速度设置 
#define SPI_SPEED_2   0
#define SPI_SPEED_8   1
#define SPI_SPEED_16  2
#define SPI_SPEED_256 3

void spi1_init(void );
uint8_t SPI1_RW(uint16_t TxData);
void SPIx_SetSpeed(u8 SpeedSet);

#endif
