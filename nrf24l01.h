#ifndef _NRF24L01_H
#define _NRF24L01_H
#include "stm32f10x.h"
#include "stdio.h"	
#include "sys.h" 
#include "spi.h"	
#include "stm32f10x_spi.h"
#include "uart.h"
#include "led.h"

#define _256kbps 0x27
#define _1Mbps   0x07
#define _2Mbps   0x0f



/******************************************************************************
							NRF24L01�Ĵ���ָ��
*******************************************************************************/ 
#define NRF_READ_REG    0x00  	// ���Ĵ���ָ��
#define NRF_WRITE_REG   0x20 	  // д�Ĵ���ָ��
#define ACTIVATE		    0x50  	    // follow with 0x73 to activate feature register
#define R_RX_PL_WID   	0x60	// �����ջ������ĳ���
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define W_ACK_PAYLOAD	  0xA8	// Used in RX mode.
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����
/******************************************************************************
							NRF24L01�Ĵ�����ַ
*******************************************************************************/ 
#define CONFIG          0x00  	// �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  	// �Զ�Ӧ��������
#define EN_RXADDR       0x02  	// �����ŵ�����
#define SETUP_AW        0x03  	// �շ���ַ�������
#define SETUP_RETR      0x04  	// �Զ��ط���������
#define RF_CH           0x05  	// ����Ƶ������
#define RF_SETUP        0x06  	// �������ʡ����Ĺ�������
#define STATUS    0x07  	//״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                              //bit5:���ݷ�������ж�;bit6:���������ж�;

#define MAX_TX  		0x10  //�ﵽ����ʹ����ж�
#define TX_OK   		0x20  //TX��������ж�
#define RX_OK   		0x40  //���յ������ж�

#define OBSERVE_TX      0x08  	// ���ͼ�⹦��
#define CD              0x09  	// ��ַ���           
#define RX_ADDR_P0      0x0A  	// Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  	// Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  	// Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  	// Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  	// Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  	// Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  	// ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  	// ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  	// ����Ƶ��1�������ݳ���
#define RX_PW_P2        0x13  	// ����Ƶ��2�������ݳ���
#define RX_PW_P3        0x14  	// ����Ƶ��3�������ݳ���
#define RX_PW_P4        0x15  	// ����Ƶ��4�������ݳ���
#define RX_PW_P5        0x16  	// ����Ƶ��5�������ݳ���
#define FIFO_STATUS     0x17  	// FIFOջ��ջ��״̬�Ĵ�������

//NRF2401����
#define NRF_CE_L      GPIO_ResetBits(GPIO_NRF24L01_CE,NRF24L01_CE)
#define NRF_CE_H      GPIO_SetBits(GPIO_NRF24L01_CE,NRF24L01_CE)
	
#define NRF_CSN_L     GPIO_ResetBits(GPIO_NRF24L01_CSN,NRF24L01_CSN)
#define NRF_CSN_H    GPIO_SetBits(GPIO_NRF24L01_CSN,NRF24L01_CSN)
#define NRF_IRQ_RAED			GPIO_ReadInputDataBit(GPIO_NRF24L01_IRQ,NRF24L01_IRQ)
//���ա����͵ĺ궨��
#define TX   0
#define RX   1

//#define RX_DR 6   //���������жϣ����յ���Ч���ݺ���1.д1����ж�
//#define TX_DS 5		//���ݷ�������жϡ������ݷ�����ɺ�����ж�
////����������Զ�Ӧ��ģʽ�£�ֻ�е����յ�Ӧ���źź󣬴�λ��1.д1����ж�
//#define MAX_RT 4   //�ﵽ����ط��жϡ�д1����жϡ�
//#define TX_FULL 0   //TX FIFO�Ĵ�������־��1��TX FIFO�Ĵ�������0����




//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��

void NRF_Init_Mode(uint8_t channel,uint8_t mode );
uint8_t NRF_Write_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen); //д������
uint8_t NRF_Read_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen);  //��������	

uint8_t NRF_Read_Reg(uint8_t regaddr);		                 //���Ĵ���
uint8_t NRF_Write_Reg(uint8_t regaddr, uint8_t data);      //д�Ĵ���
uint8_t NRF24L01_Check(void);                                 //���NRF24L01�Ƿ���λ
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);                 //����һ����������
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);                 //����һ����������


//void NRF24L01_Init(void);

#endif
