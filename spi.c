#include "spi.h"


void spi1_init(void ){
	
	SPI_InitTypeDef  spi_it;
    GPIO_InitTypeDef gpio_it;
	    RCC_APB2PeriphClockCmd(RCC_SPI1_PORT, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_SPI1_CLK, ENABLE);    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//�򿪸���ʱ�ӵ�
	
	gpio_it.GPIO_Pin=SPI1_SCK|SPI1_MOSI|SPI1_MISO;
	gpio_it.GPIO_Mode = GPIO_Mode_AF_PP;
  gpio_it.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SPI1_GPIO_PORT, &gpio_it);
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
	
	
	RCC_APB2PeriphClockCmd(RCC_NRF24L01_CE, ENABLE);//��ʼ�� NRF24L01��CE�˿�
	gpio_it.GPIO_Pin=NRF24L01_CE;
	gpio_it.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_it.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIO_NRF24L01_CE, &gpio_it);
	
	RCC_APB2PeriphClockCmd(RCC_NRF24L01_CSN, ENABLE);//��ʼ�� NRF24L01��CSN�˿�
	gpio_it.GPIO_Pin=NRF24L01_CSN;
	gpio_it.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_it.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIO_NRF24L01_CSN, &gpio_it);
	
	RCC_APB2PeriphClockCmd(RCC_NRF24L01_IRQ, ENABLE);//��ʼ�� NRF24L01��IRQ�˿�
	gpio_it.GPIO_Pin=NRF24L01_IRQ;
	gpio_it.GPIO_Mode=GPIO_Mode_IPU;
	gpio_it.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIO_NRF24L01_IRQ, &gpio_it);
	
     NRF_CSN_H; 	                                        //ʹ��24L01
	  spi_it.SPI_Mode = SPI_Mode_Master;                       //����SPI1Ϊ��ģʽ
    spi_it.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI1����Ϊ����ȫ˫��
    spi_it.SPI_DataSize = SPI_DataSize_8b;                   //SPI���ͽ���8λ֡�ṹ
    spi_it.SPI_CPOL = SPI_CPOL_Low;                          //����ʱ���ڲ�����ʱ��ʱ��Ϊ�͵�ƽ
    spi_it.SPI_CPHA = SPI_CPHA_1Edge;                        //��һ��ʱ���ؿ�ʼ��������
    spi_it.SPI_NSS = SPI_NSS_Soft;                           //NSS�ź��������ʹ��SSIλ������
    spi_it.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;  //SPI������Ԥ��ƵֵΪ8
    spi_it.SPI_FirstBit = SPI_FirstBit_MSB;                  //���ݴ����MSBλ��ʼ
    spi_it.SPI_CRCPolynomial = 7;                            //CRCֵ����Ķ���ʽ
		
    SPI_Init(SPI1, &spi_it);                                 //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPI1�Ĵ���	
	
	  SPI_Cmd(SPI1, ENABLE);
		
 //SPI1_RW(0xff);//��������
 NRF_CE_L;
 NRF_CSN_H; 	                                        //ʹ��24L01
}
 

uint8_t SPI1_RW(uint16_t TxData){
	
	uint8_t retry=0;				 	
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)      //�ȴ����ͻ����־λΪ1
    {
        retry++;
        if(retry>200)return 0;
    }			  
    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI1, TxData);                                     //ͨ������SPI1����һ������
    retry=0;
    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)     //�ȴ����ջ����־λΪ��1
    {
        retry++;
        if(retry>200)return 0;
    }	  						    
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);                                    //ͨ��SPI1���ؽ�������
	
	
}
