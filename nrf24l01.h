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
							NRF24L01寄存器指令
*******************************************************************************/ 
#define NRF_READ_REG    0x00  	// 读寄存器指令
#define NRF_WRITE_REG   0x20 	  // 写寄存器指令
#define ACTIVATE		    0x50  	    // follow with 0x73 to activate feature register
#define R_RX_PL_WID   	0x60	// 读接收缓冲区的长度
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define W_ACK_PAYLOAD	  0xA8	// Used in RX mode.
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP             0xFF  	// 保留
/******************************************************************************
							NRF24L01寄存器地址
*******************************************************************************/ 
#define CONFIG          0x00  	// 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  	// 自动应答功能设置
#define EN_RXADDR       0x02  	// 可用信道设置
#define SETUP_AW        0x03  	// 收发地址宽度设置
#define SETUP_RETR      0x04  	// 自动重发功能设置
#define RF_CH           0x05  	// 工作频率设置
#define RF_SETUP        0x06  	// 发射速率、功耗功能设置
#define STATUS    0x07  	//状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
                              //bit5:数据发送完成中断;bit6:接收数据中断;

#define MAX_TX  		0x10  //达到最大发送次数中断
#define TX_OK   		0x20  //TX发送完成中断
#define RX_OK   		0x40  //接收到数据中断

#define OBSERVE_TX      0x08  	// 发送监测功能
#define CD              0x09  	// 地址检测           
#define RX_ADDR_P0      0x0A  	// 频道0接收数据地址
#define RX_ADDR_P1      0x0B  	// 频道1接收数据地址
#define RX_ADDR_P2      0x0C  	// 频道2接收数据地址
#define RX_ADDR_P3      0x0D  	// 频道3接收数据地址
#define RX_ADDR_P4      0x0E  	// 频道4接收数据地址
#define RX_ADDR_P5      0x0F  	// 频道5接收数据地址
#define TX_ADDR         0x10  	// 发送地址寄存器
#define RX_PW_P0        0x11  	// 接收频道0接收数据长度
#define RX_PW_P1        0x12  	// 接收频道1接收数据长度
#define RX_PW_P2        0x13  	// 接收频道2接收数据长度
#define RX_PW_P3        0x14  	// 接收频道3接收数据长度
#define RX_PW_P4        0x15  	// 接收频道4接收数据长度
#define RX_PW_P5        0x16  	// 接收频道5接收数据长度
#define FIFO_STATUS     0x17  	// FIFO栈入栈出状态寄存器设置

//NRF2401控制
#define NRF_CE_L      GPIO_ResetBits(GPIO_NRF24L01_CE,NRF24L01_CE)
#define NRF_CE_H      GPIO_SetBits(GPIO_NRF24L01_CE,NRF24L01_CE)
	
#define NRF_CSN_L     GPIO_ResetBits(GPIO_NRF24L01_CSN,NRF24L01_CSN)
#define NRF_CSN_H    GPIO_SetBits(GPIO_NRF24L01_CSN,NRF24L01_CSN)
#define NRF_IRQ_RAED			GPIO_ReadInputDataBit(GPIO_NRF24L01_IRQ,NRF24L01_IRQ)
//接收、发送的宏定义
#define TX   0
#define RX   1

//#define RX_DR 6   //接收数据中断，接收到有效数据后置1.写1清除中断
//#define TX_DS 5		//数据发送完成中断。当数据发送完成后产生中断
////如果工作在自动应答模式下，只有当接收到应答信号后，此位置1.写1清楚中断
//#define MAX_RT 4   //达到最多重发中断。写1清楚中断。
//#define TX_FULL 0   //TX FIFO寄存器满标志。1：TX FIFO寄存器满；0：空




//24L01发送接收数据宽度定义
#define TX_ADR_WIDTH    5   	//5字节的地址宽度
#define RX_ADR_WIDTH    5   	//5字节的地址宽度
#define TX_PLOAD_WIDTH  32  	//32字节的用户数据宽度
#define RX_PLOAD_WIDTH  32  	//32字节的用户数据宽度

void NRF_Init_Mode(uint8_t channel,uint8_t mode );
uint8_t NRF_Write_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen); //写数据区
uint8_t NRF_Read_Buf(uint8_t regaddr, uint8_t *pBuf, uint8_t datalen);  //读数据区	

uint8_t NRF_Read_Reg(uint8_t regaddr);		                 //读寄存器
uint8_t NRF_Write_Reg(uint8_t regaddr, uint8_t data);      //写寄存器
uint8_t NRF24L01_Check(void);                                 //检查NRF24L01是否在位
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);                 //发送一个包的数据
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);                 //接收一个包的数据


//void NRF24L01_Init(void);

#endif
