#include "spi.h"


void spi1_init(void ){
	
	SPI_InitTypeDef  spi_it;
    GPIO_InitTypeDef gpio_it;
	    RCC_APB2PeriphClockCmd(RCC_SPI1_PORT, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_SPI1_CLK, ENABLE);    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//打开复用时钟等
	
	gpio_it.GPIO_Pin=SPI1_SCK|SPI1_MOSI|SPI1_MISO;
	gpio_it.GPIO_Mode = GPIO_Mode_AF_PP;
  gpio_it.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI1_GPIO_PORT, &gpio_it);
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
	
	
	RCC_APB2PeriphClockCmd(RCC_NRF24L01_CE, ENABLE);//初始化 NRF24L01的CE端口
	gpio_it.GPIO_Pin=NRF24L01_CE;
	gpio_it.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_it.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIO_NRF24L01_CE, &gpio_it);
	
	RCC_APB2PeriphClockCmd(RCC_NRF24L01_CSN, ENABLE);//初始化 NRF24L01的CSN端口
	gpio_it.GPIO_Pin=NRF24L01_CSN;
	gpio_it.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_it.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIO_NRF24L01_CSN, &gpio_it);
	
	RCC_APB2PeriphClockCmd(RCC_NRF24L01_IRQ, ENABLE);//初始化 NRF24L01的IRQ端口
	gpio_it.GPIO_Pin=NRF24L01_IRQ;
	gpio_it.GPIO_Mode=GPIO_Mode_IPU;
	gpio_it.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIO_NRF24L01_IRQ, &gpio_it);
	
     NRF_CSN_H; 	                                        //使能24L01
	  spi_it.SPI_Mode = SPI_Mode_Master;                       //设置SPI1为主模式
    spi_it.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1设置为两线全双工
    spi_it.SPI_DataSize = SPI_DataSize_8b;                   //SPI发送接收8位帧结构
    spi_it.SPI_CPOL = SPI_CPOL_Low;                          //串行时钟在不操作时，时钟为低电平
    spi_it.SPI_CPHA = SPI_CPHA_1Edge;                        //第一个时钟沿开始采样数据
    spi_it.SPI_NSS = SPI_NSS_Soft;                           //NSS信号由软件（使用SSI位）管理
    spi_it.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //SPI波特率预分频值为8
    spi_it.SPI_FirstBit = SPI_FirstBit_MSB;                  //数据传输从MSB位开始
    spi_it.SPI_CRCPolynomial = 7;                            //CRC值计算的多项式
		
    SPI_Init(SPI1, &spi_it);                                 //根据SPI_InitStruct中指定的参数初始化外设SPI1寄存器	
	
	  SPI_Cmd(SPI1, ENABLE);
		
 //SPI1_RW(0xff);//启动传输
 NRF_CE_L;
 NRF_CSN_H; 	                                        //使能24L01
}
 

uint8_t SPI1_RW(uint16_t TxData){
	
	uint8_t retry=0;				 	
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)      //等待发送缓存标志位为1
    {
        retry++;
        if(retry>200)return 0;
    }			  
    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI1, TxData);                                     //通过外设SPI1发送一个数据
    retry=0;
    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)     //等待接收缓存标志位为空1
    {
        retry++;
        if(retry>200)return 0;
    }	  						    
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);                                    //通过SPI1返回接收数据
	
	
}
